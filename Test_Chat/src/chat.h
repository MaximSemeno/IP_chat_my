#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>

class IPv4Chat {
public:
    IPv4Chat(const std::string& ip, int port);
    ~IPv4Chat();
    
    void run();
    
private:
    void receiverThread();
    void senderThread();
    
    std::string ip_;
    int port_;
    std::string nickname_;
    std::atomic<bool> running_;
    int sockfd_;
    struct sockaddr_in broadcast_addr_;
    std::thread receiver_thread_;
    std::thread sender_thread_;
};

#endif // CHAT_H