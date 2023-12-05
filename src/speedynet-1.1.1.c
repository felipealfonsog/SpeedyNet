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
#define MAX_CONNECT_RETRIES 5

int sockfd;
int interrupted = 0;

void cleanup() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}

void sigIntHandler(int signo) {
    interrupted = 1;
}

void displayIntro() {
    // (Código de presentación)
}

void displayLicense() {
    // (Código de licencia)
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
    double last_progress = 0.0;

    // Set up the signal handler
    signal(SIGINT, sigIntHandler);

    for (int i = 0; i < TEST_DURATION * 10 && !interrupted; i++) {
        int send_result = send(sockfd, buffer, sizeof(buffer), 0);
        int recv_result = recv(sockfd, buffer, sizeof(buffer), 0);

        if (send_result < 0 || recv_result < 0) {
            perror("Error sending/receiving data during speed test");
            cleanup();
            return 1;
        }

        double progress = (i + 1) / (double)(TEST_DURATION * 10) * 100;
        if (progress > last_progress) {
            last_progress = progress;
            printf("In process... %.2f%% complete\r", progress);
            fflush(stdout);
        }

        // Check if interrupted or timed out
        time_t current_timestamp = time(NULL);
        if (interrupted || difftime(current_timestamp, start_timestamp) > 10.0) {
            printf("\nTest interrupted or timed out.\n");
            break;
        }
    }

    end_time = clock();
    total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    double download_speed = (double)(BUFFER_SIZE * TEST_DURATION * 10) / (total_time * 1024 * 1024);
    double upload_speed = (double)(BUFFER_SIZE * TEST_DURATION * 10) / (total_time * 1024 * 1024);

    printf("\nTest completed.\n");
    printf("Selected Server: %s\n", selected_server);
    printf("Download Speed: %.2f Mbps\n", download_speed);
    printf("Upload Speed: %.2f Mbps\n", upload_speed);

    cleanup();

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
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Error opening socket for speed test");
            return 1;
        }

        struct sockaddr_in server_addr;
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

    return performSpeedTest(selected_server);
}
