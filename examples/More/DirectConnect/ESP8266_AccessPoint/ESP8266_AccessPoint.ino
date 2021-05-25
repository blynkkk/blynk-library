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
  This sketch shows how to connect to your ESP8266 directly from Blynk App

  NOTE: This requires ESP8266 support package:
       https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
    in the Tools -> Board menu!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// This device Access Point credentials.
// Set password to "" to create open network.
const char* ssid = "YourNetworkName";
const char* pass = "YourPassword";

WiFiServer wifiServer(80);
WiFiClient wifiClient;

// This function tries to reconnect to WiFi network
void connectWiFi() {
  Serial.print("Configuring access point...");

  if (pass && strlen(pass)) {
    WiFi.softAP((char*)ssid, (char*)pass);
  } else {
    WiFi.softAP((char*)ssid);
  }

  wifiServer.stop();

  wifiServer.begin();

  Serial.print("Server started: ");
  Serial.println(WiFi.softAPIP());
}

void setup() {
  Serial.begin(9600);

  connectWiFi();
}

void loop() {
  // If thereis some client
  wifiClient = wifiServer.available();
  if (wifiClient)
  {
    Serial.println("client available");
    Blynk.begin(wifiClient, auth);
    while (wifiClient.connected())
    {
      Blynk.run();
    }
  }
}

