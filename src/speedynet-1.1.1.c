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
#include <signal.h>

#define BUFFER_SIZE 1024
#define TEST_DURATION 5
#define MAX_CONNECT_RETRIES 5

int sockfd;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void cleanup() {
    if (sockfd >= 0) {
        close(sockfd);
    }
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
    printf("Version: - with love from Chile\n");
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

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    return size * nmemb;
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    clock_t start_time, end_time;
    double total_time;

    const char *server_list[] = {
        "8.8.8.8",   // Google DNS
        "1.1.1.1",   // Cloudflare DNS,
    };
    int num_servers = sizeof(server_list) / sizeof(server_list[0]);

    CURL *curl = curl_easy_init();

    // Fetch server list from Speedtest API
    if (curl) {
        char api_url[] = "https://www.speedtest.net/api/v2/servers";
        curl_easy_setopt(curl, CURLOPT_URL, api_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("Error fetching server list from API.\n");
            curl_easy_cleanup(curl);
            return 1;
        }
        curl_easy_cleanup(curl);
    }

    srand(time(NULL));
    const char *selected_server = NULL;

    int connection_failed = 0;
    for (int i = 0; i < num_servers; i++) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            printf("Error opening socket for speed test.\n");
            return 1;
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(80);
        server_addr.sin_addr.s_addr = inet_addr(server_list[i]);

        int connect_retries = 0;
        while (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            if (connect_retries >= MAX_CONNECT_RETRIES) {
                connection_failed = 1;
                break;
            }
            connect_retries++;
            usleep(1000000);  // Wait for 1 second before retrying
        }

        if (!connection_failed) {
            selected_server = server_list[i];
            break;
        }

        close(sockfd);
    }

    if (connection_failed) {
        selected_server = "8.8.8.8";  // Use a default server if all options failed
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error opening socket for speed test.\n");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = inet_addr(selected_server);

    int connect_result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connect_result < 0) {
        printf("Error connecting to server for speed test.\n");
        return 1;
    }

    start_time = clock();
    for (int i = 0; i < BUFFER_SIZE; i++) buffer[i] = 'a';

    displayIntro();
    printf("Performing speed test...\n");

    for (int i = 0; i < TEST_DURATION * 10; i++) {
        int send_result = send(sockfd, buffer, sizeof(buffer), 0);
        int recv_result = recv(sockfd, buffer, sizeof(buffer), 0);

        if (send_result < 0 || recv_result < 0) {
            printf("Error sending/receiving data during speed test.\n");
            close(sockfd);
            return 1;
        }

        double progress = (i + 1) / (double)(TEST_DURATION * 10) * 100;
        printf("In process... %.2f%% complete\r", progress);
        fflush(stdout);
        usleep(100000);  // Wait for 100 milliseconds
    }
    printf("\n");

    end_time = clock();
    total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    double download_speed = (double)(BUFFER_SIZE * TEST_DURATION * 10) / (total_time * 1024 * 1024);
    double upload_speed = (double)(BUFFER_SIZE * TEST_DURATION * 10) / (total_time * 1024 * 1024);

    printf("\nTest completed.\n");
    printf("Selected Server: %s\n", selected_server);
    printf("Download Speed: %.2f Mbps\n", download_speed);
    printf("Upload Speed: %.2f Mbps\n", upload_speed);

    if (connection_failed) {
        printf("Connection Status: Failed\n");
    }

    displayLicense();

    close(sockfd);

    return 0;
}
