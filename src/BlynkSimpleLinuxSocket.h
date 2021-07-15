/**
 * @file       BlynkSimpleLinuxSocket.h
 * @author     Hyeonki Hong(hhk7734@gmail.com)
 * @license    This project is released under the MIT License (MIT)
 * @date       Jul 2019
 * @brief      For Arduino Core running on the embedded Linux. ex) Odroid
 */

#ifndef BlynkSimpleLinuxSocket_h
#define BlynkSimpleLinuxSocket_h

/// @bug if you change the include order, get a compiling error
#include <../linux/BlynkSocket.h>
#include <BlynkApiArduino.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket                 Blynk( _blynkTransport );

#include <BlynkWidgets.h>

/**
 * @fn void begin(const char* auth,
                  const char* domain = BLYNK_DEFAULT_DOMAIN,
                  uint16_t    port   = BLYNK_DEFAULT_PORT)
 * @param const char* auth : auth token
 * @param const char* domain : blynk server domain, default - BLYNK_DEFAULT_DOMAIN
 * @param uint16_t port : blynk server port, default - BLYNK_DEFAULT_PORT
 */

#endif // BlynkSimpleLinuxSocket_h