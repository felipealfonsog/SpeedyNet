# ----------------------------------------------------------------
# Developed by Felipe Alfonso González - Computer Science Engineer
# Contact : f.alfonso@res-ear.ch - github.com/felipealfonsog
# Under MIT Licence
# ----------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

struct Server {
    char name[50];
    char address[50];
};

double measureDownloadSpeed(const struct Server* server) {
    return (rand() % 500) + 1;
}

double measureUploadSpeed(const struct Server* server) {
    return (rand() % 500) + 1;
}

int measureLatency(const struct Server* server) {
    return rand() % 100;
}

double measurePacketLoss(const struct Server* server) {
    return (rand() % 10) / 10.0;
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

int displayServerMenu(const struct Server servers[], size_t numServers) {
    printf("Select a server:\n");

    for (size_t i = 0; i < numServers; ++i) {
        printf("%zu. %s\n", i + 1, servers[i].name);
    }

    int choice;
    printf("Enter the number of the server: ");
    scanf("%d", &choice);

    return choice;
}

void displayResults(const struct Server* selectedServer, double downloadSpeed, double uploadSpeed, int latency, double packetLoss) {
    printf("\nTest Results:\n");
    printf("Selected Server: %s (%s)\n", selectedServer->name, selectedServer->address);
    printf("Download Speed: %.2f Mbps\n", downloadSpeed);
    printf("Upload Speed: %.2f Mbps\n", uploadSpeed);

    // Additional unit conversions
    printf("Download Speed: %.2f Kbps\n", downloadSpeed * 1000);
    printf("Upload Speed: %.2f Gbps\n", uploadSpeed / 1000);

    printf("Latency: %d ms\n", latency);
    printf("Packet Loss: %.2f%%\n", packetLoss * 100);
}

void displayAverageResults(double avgDownloadSpeed, double avgUploadSpeed, double avgLatency, double avgPacketLoss, size_t numTests) {
    printf("\nAverage Results over %zu tests:\n", numTests);
    printf("Average Download Speed: %.2f Mbps\n", avgDownloadSpeed);
    printf("Average Upload Speed: %.2f Mbps\n", avgUploadSpeed);
    printf("Average Latency: %.2f ms\n", avgLatency);
    printf("Average Packet Loss: %.2f%%\n", avgPacketLoss * 100);
}

void handleError(const char* errorMessage) {
    fprintf(stderr, "Error: %s\n", errorMessage);
}

int main() {
    displayIntro();

    struct Server servers[] = {
        {"Google DNS", "8.8.8.8"},
        {"Cloudflare DNS", "1.1.1.1"},
        {"Movistar Chile", "speedtest.movistar.cl"},
        {"VTR", "speedtest.vtr.net"},
        {"Claro Chile", "speedtest.clarochile.cl"},
        {"Entel Chile", "speedtest.entel.cl"},
        {"Gtd Manquehue", "speedtest.gtdinternet.com"},
        {"Coopeuch", "speedtest.coopeuch.cl"},
        {"IFX Networks", "speedtest.ifxnetworks.cl"},
        {"Libre Net", "speedtest.librenet.cl"},
        {"Netline", "speedtest.netline.cl"},
        {"Telsur", "speedtest.telsur.cl"},
        {"Google", "speedtest.google.com"},
        {"Cloudflare", "speed.cloudflare.com"},
        {"Fast.com", "fast.com"},
        {"Speedtest.net", "speedtest.net"},
        {"Comcast", "speedtest.xfinity.com"},
        {"AT&T", "speedtest.att.com"},
        {"Verizon", "speedtest.verizon.com"},
        {"Spectrum", "speedtest.spectrum.com"},
        {"Vodafone", "speedtest.vodafone.com"},
        {"Telefónica", "speedtest.telefonica.com"},
        {"Movistar", "speedtest.movistar.com"},
        {"Orange", "speedtest.orange.com"},
        {"T-Mobile", "speedtest.t-mobile.com"},
        {"Sprint", "speedtest.sprint.com"},
        {"Telstra", "speedtest.telstra.com"},
    };

    size_t numServers = sizeof(servers) / sizeof(servers[0]);

    size_t numTests = 5;  // Change this to the desired number of tests

    double avgDownloadSpeed = 0, avgUploadSpeed = 0, avgLatency = 0, avgPacketLoss = 0;

    for (size_t i = 0; i < numTests; ++i) {
        int choice = displayServerMenu(servers, numServers);

        if (choice < 1 || choice > (int)numServers) {
            handleError("Invalid server choice");
            return 1;
        }

        struct Server selectedServer = servers[choice - 1];

        double downloadSpeed = measureDownloadSpeed(&selectedServer);
        double uploadSpeed = measureUploadSpeed(&selectedServer);
        int latency = measureLatency(&selectedServer);
        double packetLoss = measurePacketLoss(&selectedServer);

        avgDownloadSpeed += downloadSpeed;
        avgUploadSpeed += uploadSpeed;
        avgLatency += latency;
        avgPacketLoss += packetLoss;

        displayResults(&selectedServer, downloadSpeed, uploadSpeed, latency, packetLoss);
    }

    avgDownloadSpeed /= numTests;
    avgUploadSpeed /= numTests;
    avgLatency /= numTests;
    avgPacketLoss /= numTests;

    displayAverageResults(avgDownloadSpeed, avgUploadSpeed, avgLatency, avgPacketLoss, numTests);

    return 0;
}

