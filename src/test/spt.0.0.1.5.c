#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/time.h>
#include <curl/curl.h>
#include <math.h>
#include <netinet/ip_icmp.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define TEST_DURATION 5

int sockfd;
int packet_loss_received = 0;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    return size * nmemb;
}

void displayIntro() {
    printf("\n");
    printf("╔══╗──────╔╗─╔═╦╗─╔╗\n");
    printf("║══╬═╦═╦═╦╝╠╦╣║║╠═╣╚╗\n");
    printf("╠══║╬║╩╣╩╣╬║║║║║║╩╣╔╣\n");
    printf("╚══╣╔╩═╩═╩═╬╗╠╩═╩═╩═╝\n");
    printf("───╚╝──────╚═╝\n");
    printf("************************************\n");
    printf("*           SpeedyNet              *\n");
    printf("*   Internet Speed Test Utility    *\n");
    printf("************************************\n");
    printf("\n");
    printf("Version: v.0.0.2 - with love from Chile\n");
    printf("Developed by Felipe Alfonso González - Computer Science Engineer\n");
    printf("Contact: f.alfonso@res-ear.ch\n");
    printf("GitHub: github.com/felipealfonsog\n");
    printf("\n");
}

void displayLicense() {
    printf("\n");
    printf("This software is licensed under the MIT License.\n");
    printf("\n");
}

unsigned short in_cksum(unsigned short *addr, int len) {
    int nleft = len;
    int sum = 0;
    unsigned short *w = addr;
    unsigned short answer = 0;

    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}

double calculateLatency(const char *server) {
    struct timeval start_time, end_time;
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Error opening socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    if (inet_pton(AF_INET, server, &server_addr.sin_addr) <= 0) error("Invalid address");

    gettimeofday(&start_time, NULL);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(sockfd);
        return -1.0; // Indicate connection failure
    }

    gettimeofday(&end_time, NULL);

    close(sockfd);

    double latency = (double)(end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                     (double)(end_time.tv_usec - start_time.tv_usec) / 1000.0;

    return latency;
}

void sigIntHandler(int signo) {
    packet_loss_received++;
    if (packet_loss_received >= 3) {
        printf("\n\nPacket loss test completed.\n");
        displayLicense();
        close(sockfd);
        exit(0);
    }
}

double calculatePacketLoss(const char *server) {
    struct sockaddr_in server_addr;
    int sockfd;
    int seq_num = 1;
    int ttl = 64;  // Time-to-Live (TTL) value for the packet

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) error("Error opening socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, server, &server_addr.sin_addr) <= 0) error("Invalid address");

    // Set TTL
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("Error setting TTL");
        close(sockfd);
        exit(1);
    }

    // Install the signal handler for SIGINT
    signal(SIGINT, sigIntHandler);

    while (1) {
        struct icmphdr icmp_header;
        memset(&icmp_header, 0, sizeof(icmp_header));

        icmp_header.type = ICMP_ECHO;
        icmp_header.code = 0;
        icmp_header.un.echo.id = getpid();
        icmp_header.un.echo.sequence = seq_num++;

        icmp_header.checksum = 0;
        icmp_header.checksum = in_cksum((unsigned short *)&icmp_header, sizeof(icmp_header));

        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);

        sendto(sockfd, &icmp_header, sizeof(icmp_header), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        char recv_buffer[BUFFER_SIZE];
        ssize_t num_bytes = recv(sockfd, recv_buffer, sizeof(recv_buffer), 0);
        if (num_bytes > 0) {
            gettimeofday(&end_time, NULL);

            if (end_time.tv_sec - start_time.tv_sec < 1) {
                packet_loss_received--;
            }
        }
        usleep(1000000);  // Wait for 1 second
    }

    return 0.0;  // Return 0.0 for demonstration purposes
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    clock_t start_time, end_time;
    double total_time;

    const char *server_list[] = {
        "8.8.8.8",   // Google DNS
        "1.1.1.1",   // Cloudflare DNS
    };
    int num_servers = sizeof(server_list) / sizeof(server_list[0]);

    CURL *curl = curl_easy_init();
    if (!curl) return 1;

    char api_url[] = "https://www.speedtest.net/api/v2/servers";
    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) return 1;
    curl_easy_cleanup(curl);

    srand(time(NULL));
    const char *selected_server = num_servers > 0 ? server_list[rand() % num_servers] : "8.8.8.8";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Error opening socket");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    if (inet_pton(AF_INET, selected_server, &server_addr.sin_addr) <= 0) error("Invalid address");

    int connect_result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    int connection_failed = 0;

    if (connect_result < 0) {
        connection_failed = 1;
    }

    start_time = clock();
    for (int i = 0; i < BUFFER_SIZE; i++) buffer[i] = 'a';

    for (int i = 0; i < TEST_DURATION * 1000; i++) {
        send(sockfd, buffer, sizeof(buffer), 0);
        recv(sockfd, buffer, sizeof(buffer), 0);
    }

    end_time = clock();
    total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    double download_speed = (double)(BUFFER_SIZE * TEST_DURATION * 1000) / (total_time * 1024 * 1024);
    double upload_speed = (double)(BUFFER_SIZE * TEST_DURATION * 1000) / (total_time * 1024 * 1024);
    double latency = connection_failed ? -1.0 : calculateLatency(selected_server);
    double packet_loss = calculatePacketLoss(selected_server);

    // Display the user interface
    displayIntro();
    printf("Selected Server: %s\n", selected_server);
    printf("Download Speed: %.2f Mbps\n", download_speed);
    printf("Upload Speed: %.2f Mbps\n", upload_speed);

    if (connection_failed) {
        printf("Connection Status: Failed\n");
    } else {
        printf("Latency: %.2f ms\n", latency);
        printf("Packet Loss: %.2f%%\n", packet_loss * 100.0);
    }

    displayLicense();

    close(sockfd);

    return 0;
}