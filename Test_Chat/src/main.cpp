#include "chat.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }
    
    std::string ip(argv[1]);
    int port = std::stoi(argv[2]);
    
    IPv4Chat chat(ip, port);
    chat.run();
    
    // Wait for threads to finish (they won't in this simple implementation)
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}