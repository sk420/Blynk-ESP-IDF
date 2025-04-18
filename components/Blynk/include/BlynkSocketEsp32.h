#ifndef BLYNK_SOCKET_ESP32_H
#define BLYNK_SOCKET_ESP32_H

#include "esp_log.h"
#include "lwip/sockets.h"
#include "Blynk/BlynkProtocol.h"
#include "Blynk/BlynkApi.h"

class BlynkTransportEsp32 {
public:
    BlynkTransportEsp32() : sockfd(-1), domain(nullptr), port(0) {}

    void begin(const char* host, uint16_t port) {
        domain = host;
        this->port = port;
    }

    bool connect() {
        struct sockaddr_in serv_addr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) return false;

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        inet_pton(AF_INET, domain, &serv_addr.sin_addr);

        if (::connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            ::close(sockfd);
            sockfd = -1;
            return false;
        }

        return true;
    }

    void disconnect() {
        if (sockfd >= 0) {
            ::close(sockfd);
            sockfd = -1;
        }
    }

    size_t read(void* buf, size_t len) {
        return recv(sockfd, buf, len, 0);
    }

    size_t write(const void* buf, size_t len) {
        return send(sockfd, buf, len, 0);
    }

    bool connected() {
        return sockfd >= 0;
    }

    int available() {
        fd_set fds;
        struct timeval timeout = { 0, 0 };
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);
        return select(sockfd + 1, &fds, NULL, NULL, &timeout) > 0;
    }

private:
    int sockfd;
    const char* domain;
    uint16_t port;
};

class BlynkSocket : public BlynkProtocol<BlynkTransportEsp32> {
    typedef BlynkProtocol<BlynkTransportEsp32> Base;
public:
    BlynkSocket(BlynkTransportEsp32& transp) : Base(transp) {}

    void begin(const char* auth, const char* domain, uint16_t port) {
        Base::begin(auth);
        this->conn.begin(domain, port);
    }
};


#endif
