/**
 * @file       BlynkParam.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkArduinoClient_h
#define BlynkArduinoClient_h

#include <BlynkApiArduino.h>
#include <Client.h>
#include <Blynk/BlynkDebug.h>

class BlynkArduinoClient
{
public:
    BlynkArduinoClient(Client& client)
        : client(client), domain(NULL), port(0)
    {
    	client.setTimeout(3000);
    }

    void begin(IPAddress a, uint16_t p) {
        domain = NULL;
        port = p;
        addr = a;
    }

    void begin(const char* d, uint16_t p) {
        domain = d;
        port = p;
    }

    bool connect() {
        if (domain) {
            BLYNK_LOG("Connecting to %s:%d", domain, port);
            return (1 == client.connect(domain, port));
        } else if (addr) {
            BLYNK_LOG("Connecting to %d.%d.%d.%d:%d", addr[0], addr[1], addr[2], addr[3], port);
            return (1 == client.connect(addr, port));
        }
        return 0;
    }

    void disconnect() { client.stop(); }

#ifdef BLYNK_ENC28J60_FIX
    size_t read(void* buf, size_t len) {
    	while (client.available() < len) { }
    	return client.read((uint8_t*)buf, len);
    }
#else
    size_t read(void* buf, size_t len) {
        return client.readBytes((char*)buf, len);
    }
#endif

#ifdef BLYNK_RETRY_SEND
    size_t write(const void* buf, size_t len) {
    	size_t sent = 0;
    	int retry = 0;
    	while (sent < len && ++retry < 10) {
    		size_t w = client.write((const uint8_t*)buf+sent, len-sent);
    		if (w != 0 && w != -1) {
    			sent += w;
    		} else {
        		delay(50);
#ifdef BLYNK_DEBUG
    			BLYNK_LOG("Retry %d send (%d/%d)", retry, sent, len);
#endif
    		}
    	}
        return sent;
    }
#else
    size_t write(const void* buf, size_t len) {
        return client.write((const uint8_t*)buf, len);
    }
#endif

    void flush() { client.flush(); }
    bool connected() { return client.connected(); }
    int available() { return client.available(); }

private:
    Client&     client;
    IPAddress   addr;
    const char* domain;
    uint16_t    port;
};

#endif
