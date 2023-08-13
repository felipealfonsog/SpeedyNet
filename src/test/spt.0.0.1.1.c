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

#define BUFFER_SIZE 1024
#define TEST_DURATION 5

void error(const char *msg) {
    perror(msg);
    exit(1);
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    return size * nmemb;
}

void displayIntro() {
    printf("\n");
    printf("************************************\n");
    printf("*           SpeedyNet              *\n");
    printf("*   Internet Speed Test Utility    *\n");
    printf("************************************\n");
    printf("\n");
}

void displayLicense() {
    printf("\n");
    printf("This software is licensed under the MIT License.\n");
    printf("\n");
}

double calculateLatency(const char *server) {
    struct timeval start_time, end_time;
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) exit(1);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    if (inet_pton(AF_INET, server, &server_addr.sin_addr) <= 0) exit(1);

    gettimeofday(&start_time, NULL);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) exit(1);

    gettimeofday(&end_time, NULL);

    close(sockfd);

    double latency = (double)(end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                     (double)(end_time.tv_usec - start_time.tv_usec) / 1000.0;

    return latency;
}

double calculatePacketLoss(const char *server) {
    // Simulate packet loss calculation
    // Replace this with actual implementation
    return 0.5; // 50% packet loss for demonstration purposes
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
    if (sockfd < 0) exit(1);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    if (inet_pton(AF_INET, selected_server, &server_addr.sin_addr) <= 0) exit(1);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) exit(1);

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
    double latency = calculateLatency(selected_server);
    double packet_loss = calculatePacketLoss(selected_server);

    // Display the user interface
    displayIntro();
    printf("Selected Server: %s\n", selected_server);
    printf("Download Speed: %.2f Mbps\n", download_speed);
    printf("Upload Speed: %.2f Mbps\n", upload_speed);
    printf("Latency: %.2f ms\n", latency);
    printf("Packet Loss: %.2f%%\n", packet_loss * 100.0);
    displayLicense();

    close(sockfd);

    return 0;
}
