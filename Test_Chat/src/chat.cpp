#include "chat.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>

IPv4Chat::IPv4Chat(const std::string& ip, int port) 
    : ip_(ip), port_(port), running_(true) {
    
    // Create UDP socket
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Enable broadcast
    int broadcast_enable = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_BROADCAST, 
                   &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("setsockopt (SO_BROADCAST) failed");
        close(sockfd_);
        exit(EXIT_FAILURE);
    }
    
    // Set up broadcast address
    memset(&broadcast_addr_, 0, sizeof(broadcast_addr_));
    broadcast_addr_.sin_family = AF_INET;
    broadcast_addr_.sin_port = htons(port_);
    broadcast_addr_.sin_addr.s_addr = inet_addr("255.255.255.255");
    
    // Bind to local address
    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = inet_addr(ip_.c_str());
    local_addr.sin_port = htons(port_);
    
    if (bind(sockfd_, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind failed");
        close(sockfd_);
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Enter your nickname: ";
    std::getline(std::cin, nickname_);
}

IPv4Chat::~IPv4Chat() {
    running_ = false;
    if (receiver_thread_.joinable()) receiver_thread_.join();
    if (sender_thread_.joinable()) sender_thread_.join();
    close(sockfd_);
}

void IPv4Chat::run() {
    receiver_thread_ = std::thread(&IPv4Chat::receiverThread, this);
    sender_thread_ = std::thread(&IPv4Chat::senderThread, this);
}

void IPv4Chat::receiverThread() {
    char buffer[1024];
    struct sockaddr_in sender_addr;
    socklen_t sender_len = sizeof(sender_addr);
    
    while (running_) {
        ssize_t recv_len = recvfrom(sockfd_, buffer, sizeof(buffer) - 1, 0,
                                   (struct sockaddr*)&sender_addr, &sender_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
            continue;
        }
        
        buffer[recv_len] = '\0';
        char* message = buffer;
        char* remote_nickname = strchr(buffer, ':');
        if (remote_nickname) {
            *remote_nickname = '\0';
            remote_nickname++;
            message = strchr(remote_nickname, ':');
            if (message) {
                *message = '\0';
                message++;
                
                std::cout << "[" << inet_ntoa(sender_addr.sin_addr) << "] "
                          << remote_nickname << ": " << message << std::endl;
            }
        }
    }
}

void IPv4Chat::senderThread() {
    std::string message;
    
    while (running_) {
        std::getline(std::cin, message);
        if (message.length() > 1000) {
            std::cerr << "Message too long (max 1000 bytes)" << std::endl;
            continue;
        }
        
        std::string full_message = nickname_ + ":" + message;
        if (sendto(sockfd_, full_message.c_str(), full_message.length(), 0,
                  (struct sockaddr*)&broadcast_addr_, sizeof(broadcast_addr_)) < 0) {
            perror("sendto failed");
        }
    }
}