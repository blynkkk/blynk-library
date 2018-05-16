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

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "YourNetworkName";
const char* pass = "YourPassword";

WiFiServer wifiServer(80);
WiFiClient wifiClient;

// This function tries to reconnect to WiFi network
void connectWiFi() {
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

  wifiServer.stop();

  wifiServer.begin();

  Serial.print("Server started: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);

  connectWiFi();
}

void loop() {
  // Reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

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

