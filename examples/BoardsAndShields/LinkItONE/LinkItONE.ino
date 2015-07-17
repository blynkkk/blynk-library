/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <BlynkSimpleLinkItONE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

//----- WIFI -----
#define WIFI_SSID "YourSSID"         //  REPLACE: your network SSID (name)
#define WIFI_PASS "YourPASS"         //  REPLACE: your network password (use for WPA, or use as key for WEP)
#define WIFI_AUTH LWIFI_WPA // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.

bool connected = false;

// This function is used by Blynk to receive data
size_t BlynkStreamRead(void* buf, size_t len)
{
  return Serial.readBytes((byte*)buf, len);
}

// This function is used by Blynk to send data
size_t BlynkStreamWrite(const void* buf, size_t len)
{
  return Serial.write((byte*)buf, len);
}

void setup()
{
  // Setup your connection here.
  Serial.begin(9600);
  Blynk.begin(auth,WIFI_SSID,WIFI_PASS,WIFI_AUTH);

  do {
    connected = Blynk.connect();
  } while (!connected);
}

void loop()
{
  // Make sure that Blynk.run() is called
  // only when the connection is established
  if (connected) {
    // Okay, handle Blynk protocol
    bool hasIncomingData = (Serial.available() > 0);
    // Tell Blynk if it has incoming data
    // (this allows to skip unneeded BlynkStreamRead calls)
    if (!Blynk.run(hasIncomingData)) {
      // Error happened. No action for serial.
    }
  }
}


