/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This sketch shows how to use multiple connectivity options
  at the same time:
    - Arduino MKR1400 GSM module provides GPRS
    - Arduino MKR ETH shield provides Ethernet
  Using a standard Client interface is a convenient way to integrate
  any connectivity shield, even if it's not directly supported by Blynk.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Uncomment this to see the verbose Blynk protocol log */
//#define BLYNK_DEBUG

/* Fill in information from Blynk Device Info here */
/* Read more: https://bit.ly/BlynkSimpleAuth */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"

/* BlynkMultiClient allows attaching Blynk to the standard Arduino Client,
   and also allows multiple (up to 4) connections to be registered.
   NOTE: don't replace it with any of the BlynkSimple*.h variants */
#include <BlynkMultiClient.h>

/*
 * Ethernet
 */

// You can specify your board mac adress
byte ETH_MAC[] =        { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoECCX08.h>
#include <ArduinoBearSSL.h>

// Ethernet shield and SDcard pins
#define MKRETH_CS  5
#define SDCARD_CS  4

static EthernetClient blynkEthernetClient;
static BearSSLClient  blynkEthernetClientSSL(blynkEthernetClient);

/*
 * GSM modem
 */

// Please specify your GPRS credentials
const char SIM_PIN[]    = "";
const char GPRS_APN[]   = "internet";
const char GPRS_USER[]  = "";
const char GPRS_PASS[]  = "";

#include <MKRGSM.h>

static GSMModem         modem;
static GPRS             gprs;
static GSM              gsmAccess;
static GSMSSLClient     blynkGsmClientSSL;

/*
 * Main
 */

void connectEthernet()
{
  if (Ethernet.begin(ETH_MAC, 5000L, 500L)) {
    Serial.print("Ethernet IP: ");
    Serial.println(Ethernet.localIP());
  } else if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.");
  } else if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  } else {
    Serial.println("Ethernet: DHCP configuration failed.");
  }
}

void connectGPRS()
{
  bool gsmConnected = false;
  bool gprsConnected = false;

  const uint32_t tstart = millis();
  while (millis() - tstart < 20000) {
    if (gsmAccess.begin(SIM_PIN) == GSM_READY) {
      gsmConnected = true;
      break;
    }
    delay(1000);
  }
  while (gsmConnected && millis() - tstart < 20000) {
    if (gprs.attachGPRS(GPRS_APN, GPRS_USER, GPRS_PASS) == GPRS_READY) {
      gprsConnected = true;
      break;
    }
    delay(1000);
  }

  if (!gsmConnected) {
    Serial.println("GSM not connected.");
  } else if (!gprsConnected) {
    Serial.println("GPRS not connected.");
  } else {
    Serial.print("GPRS IP: ");
    Serial.println(gprs.getIPAddress());
  }
}

unsigned long ntpGetTime()
{
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

  for (int i = 0; i < 10; i++)
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

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Deselect the SD card
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);

  // Initialize Ethernet shield
  Ethernet.init(MKRETH_CS);

  // Enable NTP time helper (needed for SSL authentiction)
  ArduinoBearSSL.onGetTime(ntpGetTime);

  connectEthernet();
  connectGPRS();

  // Setup Blynk
  Blynk.addClient("ETH", blynkEthernetClientSSL,  443);
  Blynk.addClient("GSM", blynkGsmClientSSL,       443);

  Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop()
{
  Blynk.run();
  Ethernet.maintain();
}

