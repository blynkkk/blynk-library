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
  This sketch shows how to use an Arduino Client directly in Blynk.
  Using a standard Client interface is a convenient way to integrate
  any connectivity shield, even if it's not directly supported by Blynk.

  NOTE: Pins 10, 11, 12 and 13 are reserved for Ethernet module.
        DON'T use them in your sketch directly!

  WARNING: If you have an SD card, you may need to disable it
        by setting pin 4 to HIGH. Read more here:
        https://www.arduino.cc/en/Main/ArduinoEthernetShield
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
#include <Ethernet.h>   // For ENC28J60, replace this with <EthernetENC.h> library

// Ethernet shield and SDcard pins
#define W5100_CS   10
#define SDCARD_CS  4

static EthernetClient blynkEthernetClient;

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

void setup()
{
  // Debug console
  Serial.begin(115200);

  // Deselect the SD card
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);

  // Initialize Ethernet shield
  Ethernet.init(W5100_CS);
  delay(1000); // Give the Ethernet shield a second to initialize

  connectEthernet();

  // Setup Blynk
  Blynk.addClient("ETH", blynkEthernetClient, 80);
  Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop()
{
  Blynk.run();
  Ethernet.maintain();
}

