#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

double calculateAverageSpeed(double downloadSpeed, double uploadSpeed) {
    return (downloadSpeed + uploadSpeed) / 2;
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

void displayServerMenu(const struct Server servers[], size_t numServers) {
    printf("Select a server by entering its number:\n");

    for (size_t i = 0; i < numServers; ++i) {
        printf("%zu. %s\n", i + 1, servers[i].name);
    }

    int choice;
    printf("Enter the number of the server: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > (int)numServers) {
        printf("Invalid server choice.\n");
        exit(EXIT_FAILURE);
    }

    struct Server selectedServer = servers[choice - 1];

    printf("\n╔════════════════════════════════════╗\n");
    printf("║ Selected Server: %-20s (%s)   ║\n", selectedServer.name, selectedServer.address);
    printf("╚════════════════════════════════════╝\n");

    double downloadSpeed = measureDownloadSpeed(&selectedServer);
    double uploadSpeed = measureUploadSpeed(&selectedServer);
    int latency = measureLatency(&selectedServer);
    double packetLoss = measurePacketLoss(&selectedServer);

    printf("\n╔══════════════════════════╗\n");
    printf("║ Test Results for %s   ║\n", selectedServer.name);
    printf("╟──────────────────────────╢\n");
    printf("║ Download Speed: %.2f Mbps ║\n", downloadSpeed);
    printf("║ Upload Speed: %.2f Mbps   ║\n", uploadSpeed);
    printf("║ Latency: %d ms            ║\n", latency);
    printf("║ Packet Loss: %.2f%%        ║\n", packetLoss * 100);
    printf("╚══════════════════════════╝\n");

    // Background test with 3 other random servers
    printf("\nBackground Test with 3 Other Random Servers:\n");

    double avgDownloadSpeed = 0, avgUploadSpeed = 0, avgLatency = 0, avgPacketLoss = 0;

    for (size_t i = 0; i < 3; ++i) {
        size_t randomIndex = rand() % numServers;
        struct Server randomServer = servers[randomIndex];

        double randomDownloadSpeed = measureDownloadSpeed(&randomServer);
        double randomUploadSpeed = measureUploadSpeed(&randomServer);
        int randomLatency = measureLatency(&randomServer);
        double randomPacketLoss = measurePacketLoss(&randomServer);

        avgDownloadSpeed += randomDownloadSpeed;
        avgUploadSpeed += randomUploadSpeed;
        avgLatency += randomLatency;
        avgPacketLoss += randomPacketLoss;

        // Display the names of servers used for background tests
        printf("\n╔════════════════════════════════════╗\n");
        printf("║ Background Test Server %zu: %-11s   ║\n", i + 1, randomServer.name);
        printf("╚════════════════════════════════════╝\n");
    }

    avgDownloadSpeed /= 3;
    avgUploadSpeed /= 3;
    avgLatency /= 3;
    avgPacketLoss /= 3;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Average Results for 3 Background Tests ║\n");
    printf("╟────────────────────────────────────────╢\n");
    printf("║ Average Download Speed: %.2f Mbps      ║\n", avgDownloadSpeed);
    printf("║ Average Upload Speed: %.2f Mbps        ║\n", avgUploadSpeed);
    printf("║ Average Latency: %.2f ms               ║\n", avgLatency);
    printf("║ Average Packet Loss: %.2f%%           ║\n", avgPacketLoss * 100);
    printf("╚════════════════════════════════════════╝\n");
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

    displayServerMenu(servers, numServers);

    return 0;
}
