/**
 * @file       BlynkSocket.h
 * @author     Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 */

#ifndef BlynkSocket_h
#define BlynkSocket_h

#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>

#include <BlynkApiLinux.h>
#include <Blynk/BlynkProtocol.h>

class BlynkTransportSocket
{
public:
    BlynkTransportSocket()
        : sockfd(-1)
    {}

    void begin() {
    }

    bool connect() {
        BLYNK_LOG("Connecting to " BLYNK_DEFAULT_DOMAIN ":" TOSTRING(BLYNK_DEFAULT_PORT));

        struct addrinfo hints;
        struct addrinfo *res;  // will point to the results

        memset(&hints, 0, sizeof hints); // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

        // get ready to connect
        getaddrinfo(BLYNK_DEFAULT_DOMAIN, TOSTRING(BLYNK_DEFAULT_PORT), &hints, &res);

        if ((sockfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
        {
            BLYNK_LOG("Can't create socket");
            return false;
        }

        if (::connect(sockfd, res->ai_addr, res->ai_addrlen) < 0)
        {
            BLYNK_LOG("Can't connect to %s", BLYNK_DEFAULT_DOMAIN);
            return false;
        }

        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

        int one = 1;
        setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &one, sizeof(one));

        freeaddrinfo(res); // TODO: Leak here

        return true;
    }

    void disconnect() {
        while (::close(sockfd) < 0) {
            usleep(10000);
        }
        sockfd = -1;
    }

    size_t read(void* buf, size_t len) {
        return ::read(sockfd, buf, len);
    }

    size_t write(const void* buf, size_t len) {
        return ::write(sockfd, buf, len);
    }

    void flush()    { }
    bool connected() { return sockfd >= 0; }
    int available() { return BLYNK_MAX_READBYTES; }

protected:
    int    sockfd;
};

class BlynkSocket
    : public BlynkProtocol<BlynkTransportSocket>
{
    typedef BlynkProtocol<BlynkTransportSocket> Base;
public:
    BlynkSocket(BlynkTransportSocket& conn)
        : Base(conn)
    {}

    void begin(const char* auth)
    {
        Base::begin(auth);
    }

};

#endif
