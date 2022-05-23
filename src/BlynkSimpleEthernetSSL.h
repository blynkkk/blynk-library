/**
 * @file       BlynkSimpleEthernetSSL.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2018 Volodymyr Shymanskyy
 * @date       Sep 2018
 * @brief
 *
 */

#ifndef BlynkSimpleEthernetSSL_h
#define BlynkSimpleEthernetSSL_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "W5000"
#endif

#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetUdp.h>
#include <ArduinoECCX08.h>
#include <ArduinoBearSSL.h>

#define BLYNK_USE_SSL

#include <Adapters/BlynkEthernet.h>

static EthernetClient _blynkEthernetClient;
static BearSSLClient _blynkEthernetClientSSL(_blynkEthernetClient);

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static BlynkArduinoClient _blynkTransport(_blynkEthernetClientSSL);
  BlynkEthernet Blynk(_blynkTransport);
#else
  extern BlynkEthernet Blynk;
#endif

#include <BlynkWidgets.h>

unsigned long ntpGetTime() {
  static const char timeServer[] = "time.nist.gov";

  const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message  
  byte packetBuffer[NTP_PACKET_SIZE];

  EthernetUDP Udp;
  Udp.begin(8888);

  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  for (int i=0; i<10; i++)
  {
    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(timeServer, 123); // NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
  
    millis_time_t started = BlynkMillis();
    while (BlynkMillis() - started < 1000)
    {
      delay(100);
      if (Udp.parsePacket()) {
        // We've received a packet, read the data from it
        Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

        // the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, extract the two words:
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        // combine the four bytes (two words) into a long integer
        // this is NTP time (seconds since Jan 1 1900):
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        //Serial.print("Seconds since Jan 1 1900 = ");
        //Serial.println(secsSince1900);
          
        // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
        const unsigned long seventyYears = 2208988800UL;
        // subtract seventy years:
        unsigned long epoch = secsSince1900 - seventyYears;
  
        // print Unix time:
        Serial.print("Unix time = ");
        Serial.println(epoch);

        return epoch;
      }
    }
    Serial.println("Retry NTP");
  }
  Serial.println("NTP failed");
  return 0;
}

#endif
