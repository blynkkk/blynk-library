/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This sketch shows how to use multiple connectivity options
  at the same time:
    - Arduino MKR1400 GSM module provides GPRS
    - Arduino MKR ETH shield provides Ethernet
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

//#define BLYNK_DEBUG

/* Fill-in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#include <BlynkMultiClient.h>

/*
 * Ethernet
 */

#include <SPI.h>
#include <Ethernet.h>

static EthernetClient   blynkEthernetClient;
byte ETH_MAC[] =        { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/*
 * GSM modem
 */

#include <MKRGSM.h>

static GSMModem         modem;
static GPRS             gprs;
static GSM              gsmAccess;
static GSMClient        blynkGsmClient;

const char SIM_PIN[]      = "";
const char GPRS_APN[]     = "internet";
const char GPRS_USER[]    = "";
const char GPRS_PASS[]    = "";

/*
 * Main
 */

#define MKRETH_CS  5
#define SDCARD_CS  4

void setup()
{
  Serial.begin(115200);

  delay(1000);

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Ethernet.init(MKRETH_CS);      // Init MKR ETH shield

  /*
   * Connect Ethernet
   */
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

  /*
   * Connect GPRS
   */
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

  /*
   * Blynk
   */

  Blynk.addClient("ETH", blynkEthernetClient, 80);
  Blynk.addClient("GSM", blynkGsmClient,      80);

  Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop() {
  Blynk.run();
  Ethernet.maintain();
}
