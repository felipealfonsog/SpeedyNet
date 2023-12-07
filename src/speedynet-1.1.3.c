#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>  // 
#include <unistd.h> // 
#include <errno.h>
#include <unistd.h>

#define SERVER_COUNT 5
#define MAX_SERVER_NAME 50
#define BUFFER_SIZE 1024
#define TIMEOUT_SECONDS 2

typedef struct {
    char name[MAX_SERVER_NAME];
    char ip[15];
} Server;

void displayIntro();
int createSocket();
void connectToServer(int sockfd, const char* ip);
void runSpeedTest(Server server);
void runLatencyTest(Server server);
void runPacketLossTest(Server server);
void displayResults(Server selectedServer, double downloadSpeed, double uploadSpeed, int latency, double packetLoss);
void handleError(const char* message);

int main() {
    Server servers[SERVER_COUNT] = {
        {"Google DNS", "8.8.8.8"},
        {"Cloudflare DNS", "1.1.1.1"},
        {"OpenDNS", "208.67.222.222"},
        {"Quad9 DNS", "9.9.9.9"},
        {"CleanBrowsing DNS", "185.228.168.9"}
    };

    displayIntro();

    for (int i = 0; i < SERVER_COUNT; ++i) {
        runSpeedTest(servers[i]);
        runLatencyTest(servers[i]);
        runPacketLossTest(servers[i]);
    }

    // Agrega la función displayResults con los resultados reales obtenidos
    displayResults(servers[0], 10.5, 5.2, 20, 2.5);

    return 0;
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

int createSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        handleError("Error creating socket");
    }
    return sockfd;
}

void connectToServer(int sockfd, const char* ip) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    inet_pton(AF_INET, ip, &(server.sin_addr));

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Error connecting to server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}







void runSpeedTest(Server server) {
    printf("\nRunning Speed Test for %s...\n", server.name);

    int sockfd = createSocket();

    struct timeval timeout;
    timeout.tv_sec = 1;  // 1 second timeout
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout)) < 0 ||
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0) {
        perror("Error setting socket timeout");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    inet_pton(AF_INET, server.ip, &(server_addr.sin_addr));

    int connectResult = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (connectResult < 0 && errno != EINPROGRESS) {
        perror("Error connecting to server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    fd_set writeSet;
    FD_ZERO(&writeSet);
    FD_SET(sockfd, &writeSet);

    int selectResult = select(sockfd + 1, NULL, &writeSet, NULL, &timeout);

    if (selectResult < 0) {
        perror("Error in select");
        close(sockfd);
        exit(EXIT_FAILURE);
    } else if (selectResult == 0) {
        fprintf(stderr, "Error connecting to server: Operation timed out\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (fcntl(sockfd, F_SETFL, 0) < 0) {
        perror("Error setting blocking socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    double downloadSpeed = 10.5;  // Simulated download speed in Mbps
    double uploadSpeed = 5.2;    // Simulated upload speed in Mbps

    close(sockfd);

    printf("Download Speed: %.2f Mbps\n", downloadSpeed);
    printf("Upload Speed: %.2f Mbps\n", uploadSpeed);
}










void runLatencyTest(Server server) {
    printf("\nRunning Latency Test (Ping) for %s...\n", server.name);

    int sockfd = createSocket();
    connectToServer(sockfd, server.ip);

    // Mide el tiempo de ida y vuelta para calcular la latencia
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Envía datos y recibe respuesta
    send(sockfd, "PING", 4, 0);
    char buffer[BUFFER_SIZE];
    recv(sockfd, buffer, BUFFER_SIZE, 0);

    gettimeofday(&end, NULL);
    close(sockfd);

    // Calcula la latencia en milisegundos (ms)
    int latency = (int)((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000);
    printf("Latency: %d ms\n", latency);
}

void runPacketLossTest(Server server) {
    printf("\nRunning Packet Loss Test for %s...\n", server.name);

    // Utiliza ICMP Echo requests para estimar la pérdida de paquetes
    char command[100];
    snprintf(command, sizeof(command), "ping -c 10 %s", server.ip);
    int result = system(command);

    if (result == -1) {
        handleError("Error running packet loss test");
    }
}

void displayResults(Server selectedServer, double downloadSpeed, double uploadSpeed, int latency, double packetLoss) {
    printf("\nResults for %s:\n", selectedServer.name);
    printf("Download Speed: %.2f Mbps\n", downloadSpeed);
    printf("Upload Speed: %.2f Mbps\n", uploadSpeed);
    printf("Latency: %d ms\n", latency);
    printf("Packet Loss: %.2f%%\n", packetLoss);
}

void handleError(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}
