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
#define TEST_DURATION 5

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
    printf("************************************\n");
    printf("*        Internet Speed Test       *\n");
    printf("************************************\n");
    printf("\n");
}

int performSpeedTest(const char *selected_server) {
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    clock_t start_time, end_time;
    double total_time;

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

    start_time = clock();
    for (int i = 0; i < BUFFER_SIZE; i++) buffer[i] = 'a';

    printf("Performing speed test...\n");

    time_t start_timestamp = time(NULL);

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

        time_t current_timestamp = time(NULL);
        if (difftime(current_timestamp, start_timestamp) > 10.0) {
            printf("\nTest timed out.\n");
            break;
        }
    }

    end_time = clock();
    total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    double download_speed = (double)(total_bytes_received) / (total_time * 1024 * 1024);

    printf("\nTest completed.\n");
    printf("Selected Server: %s\n", selected_server);
    printf("Download Speed: %.2f Mbps\n", download_speed);

    cleanup();

    return 0;
}

int connectToServer(const char *server_ip) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket for speed test");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server for speed test");
        close(sockfd);
        return -1;
    }

    close(sockfd);
    return 0;
}

int main() {
    const char *server_list[] = {
        "8.8.8.8",   // Google DNS
        "1.1.1.1",   // Cloudflare DNS,
    };

    int num_servers = sizeof(server_list) / sizeof(server_list[0]);

    srand(time(NULL));
    const char *selected_server = NULL;

    int connection_failed = 0;
    for (int i = 0; i < num_servers; i++) {
        int result = connectToServer(server_list[i]);
        if (result == 0) {
            selected_server = server_list[i];
            break;
        }
    }

    if (selected_server == NULL) {
        fprintf(stderr, "Failed to connect to any server.\n");
        return 1;
    }

    displayIntro();
    return performSpeedTest(selected_server);
}
