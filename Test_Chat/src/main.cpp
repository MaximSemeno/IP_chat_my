#include "chat.h"
#include <iostream>
#include <string>
#include <regex>
#include <csignal>
#include <atomic>

std::atomic<bool> running(true);

void signalHandler(int) {
    running = false;
}

bool isValidIP(const std::string& ip) {
    std::regex ipRegex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    return std::regex_match(ip, ipRegex);
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signalHandler);
    
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }
    
    try {
        std::string ip(argv[1]);
        if (!isValidIP(ip)) {
            std::cerr << "Invalid IP address format" << std::endl;
            return 1;
        }
        
        int port = std::stoi(argv[2]);
        if (port < 0 || port > 65535) {
            std::cerr << "Port must be between 0 and 65535" << std::endl;
            return 1;
        }
        
        IPv4Chat chat(ip, port);
        chat.run();
        
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}