/**
 * @file       BlynkSocket.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 */

#ifndef BlynkSocket_h
#define BlynkSocket_h

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <Blynk/BlynkProtocol.h>

class BlynkTransportSocket
{
public:
    BlynkTransportSocket()
        : sockfd(-1), domain(NULL), port(NULL)
    {}

    void begin(const char* h, uint16_t p) {
        this->domain = h;
        this->port = p;
    }

    bool connect()
    {
        BLYNK_LOG4(BLYNK_F("Connecting to "), domain, ':', port);

        struct addrinfo hints;
        struct addrinfo *res = NULL;  // will point to the results

        memset(&hints, 0, sizeof hints); // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

        // get ready to connect
        char port_str[8];
        snprintf(port_str, sizeof(port_str), "%u", port);
        getaddrinfo(domain, port_str, &hints, &res);

        if (res == NULL) {
            BLYNK_LOG1(BLYNK_F("Cannot get addr info"));
            return false;
        }

        if ((sockfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
        {
            BLYNK_LOG1(BLYNK_F("Can't create socket"));
            return false;
        }

        if (::connect(sockfd, res->ai_addr, res->ai_addrlen) < 0)
        {
            BLYNK_LOG2(BLYNK_F("Can't connect to "), domain);
            return false;
        }

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 1000;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

        int one = 1;
        setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &one, sizeof(one));

        freeaddrinfo(res); // TODO: Leak here

        return true;
    }

    void disconnect()
    {
        if (sockfd != -1) {
            while (::close(sockfd) < 0) {
                usleep(10000);
            }
            sockfd = -1;
        }
    }

    size_t read(void* buf, size_t len) {
        ssize_t rlen = ::read(sockfd, buf, len);
        if (rlen == -1) {
            //BLYNK_LOG4("Read error ", errno, ": ", strerror(errno));
            if (errno == ETIMEDOUT || errno == EWOULDBLOCK || errno == EAGAIN) {
                return 0;
            }
            disconnect();
            return -1;
        }
        return rlen;
    }

    size_t write(const void* buf, size_t len) {
        return ::write(sockfd, buf, len);
    }

    bool connected() {
      return sockfd >= 0;
    }

    int available() {
        if (!connected()) {
            return 0;
        }

        int count = 0;
        if (0 == ioctl(sockfd, FIONREAD, &count)) {
            if (!count) {
                usleep(10000); // not to stall CPU with 100% load
            }
            return count;
        }
        return 0;
    }

protected:
    int         sockfd;
    const char* domain;
    uint16_t    port;
};

class BlynkSocket
    : public BlynkProtocol<BlynkTransportSocket>
{
    typedef BlynkProtocol<BlynkTransportSocket> Base;
public:
    BlynkSocket(BlynkTransportSocket& transp)
        : Base(transp)
    {}

    void begin(const char* auth,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
    }

};

#endif
