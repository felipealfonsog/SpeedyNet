#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define TEST_DURATION 10

int sockfd;

void cleanup() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}

void sigIntHandler(int signo) {
    printf("\nTest interrupted.\n");
    cleanup();
    exit(0);
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
    printf("With love from Chile\n");
    printf("Developed by Felipe Alfonso González - Computer Science Engineer\n");
    printf("Contact: f.alfonso@res-ear.ch\n");
    printf("GitHub: github.com/felipealfonsog\n");
        printf("\n");
    printf("This software is licensed under the MIT License.\n");
    printf("\n");
}

int performSpeedTest(const char *selected_server) {
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    struct timespec start_time, end_time;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket for speed test");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = inet_addr(selected_server);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server for speed test");
        cleanup();
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start_time);

    printf("Performing speed test...\n");

    struct sigaction sa;
    sa.sa_handler = sigIntHandler;
    sigaction(SIGINT, &sa, NULL);

    int total_bytes_received = 0;
    for (int i = 0; i < TEST_DURATION * 10; i++) {
        int send_result = send(sockfd, buffer, sizeof(buffer), 0);
        int recv_result = recv(sockfd, buffer, sizeof(buffer), 0);

        if (send_result < 0 || recv_result < 0) {
            perror("Error sending/receiving data during speed test");
            cleanup();
            return 1;
        }

        total_bytes_received += recv_result;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double total_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    double download_speed = (double)(total_bytes_received) / (total_time * 1024 * 1024);

    printf("\nTest completed.\n");
    printf("Selected Server: %s\n", selected_server);
    printf("Download Speed: %.2f Mbps\n", download_speed);

    cleanup();

    return 0;
}

// ... (unchanged)

int main() {
    // ... (unchanged)

    return performSpeedTest(selected_server);
}
