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

  Please be sure to select a correct ESP8266 or ESP32 module
    in the Tools -> Board menu!
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
 * WiFi
 */

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "YourNetworkName";
const char* pass = "YourPassword";

#ifdef ESP32
#  include <WiFi.h>
#else
#  include <ESP8266WiFi.h>
#endif

static WiFiClient blynkWiFiClient;

/*
 * Main
 */

void connectWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (pass && strlen(pass)) {
    WiFi.begin((char*)ssid, (char*)pass);
  } else {
    WiFi.begin((char*)ssid);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  connectWiFi();

  // Setup Blynk
  Blynk.addClient("WiFi", blynkWiFiClient, 80);
  Blynk.config(BLYNK_AUTH_TOKEN);
}

void loop()
{
  // Reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  Blynk.run();
}

