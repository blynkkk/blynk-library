/**
 * @file       BlynkSimpleYun.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkSimpleYun_h
#define BlynkSimpleYun_h

#ifndef BLYNK_INFO_DEVICE
#define BLYNK_INFO_DEVICE  "Arduino Yun"
#endif

#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <YunClient.h>

typedef BlynkArduinoClient BlynkArduinoClientYun;

class BlynkYun
    : public BlynkProtocol< BlynkArduinoClientYun >
{
    typedef BlynkProtocol< BlynkArduinoClientYun > Base;
public:
    BlynkYun(BlynkArduinoClientYun& transp)
        : Base(transp)
    {}

    void config(const char* auth,
            	const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
    	Base::begin(auth);
    	this->conn.begin(domain, port);
    }

    void config(const char* auth,
            	IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT)
    {
    	Base::begin(auth);
    	this->conn.begin(ip, port);
    }

    void begin(const char* auth,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
        BLYNK_LOG("Bridge init...");
        Bridge.begin();
    	config(auth, domain, port);
    }

    void begin(const char* auth,
               IPAddress   ip,
               uint16_t    port = BLYNK_DEFAULT_PORT)
    {
        BLYNK_LOG("Bridge init...");
        Bridge.begin();
    	config(auth, ip, port);
    }

};

static YunClient _blynkYunClient;
static BlynkArduinoClient _blynkTransport(_blynkYunClient);
BlynkYun Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
