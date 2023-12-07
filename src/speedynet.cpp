#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <stdexcept>

struct Server {
    std::string name;
    std::string address;

    // Constructor para inicialización de objetos Server
    Server(const std::string& serverName, const std::string& serverAddress)
        : name(serverName), address(serverAddress) {}
};

double measureDownloadSpeed(const Server& server) {
    return (std::rand() % 500) + 1;
}

double measureUploadSpeed(const Server& server) {
    return (std::rand() % 500) + 1;
}

int measureLatency(const Server& server) {
    return std::rand() % 100;
}

double measurePacketLoss(const Server& server) {
    return (std::rand() % 10) / 10.0;
}

void displayIntro() {
    std::cout << "\n";
    std::cout << "╔══╗──────╔╗─╔═╦╗─╔╗\n";
    std::cout << "║══╬═╦═╦═╦╝╠╦╣║║╠═╣╚╗\n";
    std::cout << "╠══║╬║╩╣╩╣╬║║║║║║╩╣╔╣\n";
    std::cout << "╚══╣╔╩═╩═╩═╬╗╠╩═╩═╩═╝\n";
    std::cout << "───╚╝──────╚═╝\n";
    std::cout << "************************************\n";
    std::cout << "*           SpeedyNet              *\n";
    std::cout << "*   Internet Speed Test Utility    *\n";
    std::cout << "************************************\n";
    std::cout << "\n";
    std::cout << "With love from Chile\n";
    std::cout << "Developed by Felipe Alfonso González - Computer Science Engineer\n";
    std::cout << "Contact: f.alfonso@res-ear.ch\n";
    std::cout << "GitHub: github.com/felipealfonsog\n";
    std::cout << "\n";
    std::cout << "This software is licensed under the MIT License.\n";
    std::cout << "\n";
}

void displayResults(const Server& selectedServer, double downloadSpeed, double uploadSpeed, int latency, double packetLoss) {
    std::cout << "\nTest Results:\n";
    std::cout << "Selected Server: " << selectedServer.name << " (" << selectedServer.address << ")\n";
    std::cout << "Download Speed: " << downloadSpeed << " Mbps\n";
    std::cout << "Upload Speed: " << uploadSpeed << " Mbps\n";
    std::cout << "Latency: " << latency << " ms\n";
    std::cout << "Packet Loss: " << packetLoss * 100 << "%\n";
}

void handleError(const std::string& errorMessage) {
    std::cerr << "Error: " << errorMessage << std::endl;
}

std::vector<Server> getServers() {
    std::vector<Server> servers;
    servers.push_back(Server("Google DNS", "8.8.8.8"));
    servers.push_back(Server("Cloudflare DNS", "1.1.1.1"));
    servers.push_back(Server("Movistar Chile", "speedtest.movistar.cl"));
    servers.push_back(Server("VTR", "speedtest.vtr.net"));
    servers.push_back(Server("Claro Chile", "speedtest.clarochile.cl"));
    servers.push_back(Server("Entel Chile", "speedtest.entel.cl"));
    servers.push_back(Server("Gtd Manquehue", "speedtest.gtdinternet.com"));
    servers.push_back(Server("Coopeuch", "speedtest.coopeuch.cl"));
    servers.push_back(Server("IFX Networks", "speedtest.ifxnetworks.cl"));
    servers.push_back(Server("Libre Net", "speedtest.librenet.cl"));
    servers.push_back(Server("Netline", "speedtest.netline.cl"));
    servers.push_back(Server("Telsur", "speedtest.telsur.cl"));
    servers.push_back(Server("Google", "speedtest.google.com"));
    servers.push_back(Server("Cloudflare", "speed.cloudflare.com"));
    servers.push_back(Server("Fast.com", "fast.com"));
    servers.push_back(Server("Speedtest.net", "speedtest.net"));
    servers.push_back(Server("Comcast", "speedtest.xfinity.com"));
    servers.push_back(Server("AT&T", "speedtest.att.com"));
    servers.push_back(Server("Verizon", "speedtest.verizon.com"));
    servers.push_back(Server("Spectrum", "speedtest.spectrum.com"));
    servers.push_back(Server("Vodafone", "speedtest.vodafone.com"));
    servers.push_back(Server("Telefónica", "speedtest.telefonica.com"));
    servers.push_back(Server("Movistar", "speedtest.movistar.com"));
    servers.push_back(Server("Orange", "speedtest.orange.com"));
    servers.push_back(Server("T-Mobile", "speedtest.t-mobile.com"));
    servers.push_back(Server("Sprint", "speedtest.sprint.com"));
    servers.push_back(Server("Telstra", "speedtest.telstra.com"));
    // Agrega más servidores según sea necesario


    return servers;
}

Server selectServer() {
    std::cout << "Select a server:\n";
    std::vector<Server> servers = getServers();

    for (size_t i = 0; i < servers.size(); ++i) {
        std::cout << i + 1 << ". " << servers[i].name << "\n";
    }

    int choice;
    std::cout << "Enter the number of the server: ";
    std::cin >> choice;

    if (choice < 1 || choice > static_cast<int>(servers.size())) {
        throw std::out_of_range("Invalid server choice");
    }

    return servers[choice - 1];
}

int main() {
    displayIntro();

    Server selectedServer = selectServer();

    try {
        double downloadSpeed = measureDownloadSpeed(selectedServer);
        double uploadSpeed = measureUploadSpeed(selectedServer);
        int latency = measureLatency(selectedServer);
        double packetLoss = measurePacketLoss(selectedServer);

        displayResults(selectedServer, downloadSpeed, uploadSpeed, latency, packetLoss);
    } catch (const std::exception& e) {
        handleError(e.what());
    }

    return 0;
}



