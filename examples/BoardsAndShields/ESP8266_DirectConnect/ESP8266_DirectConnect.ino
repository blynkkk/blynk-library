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
 * This example shows how to connect to ESP8266 directly
 * without using intermediate server.
 *
 * Please note: this example is for experienced users.
 * Use this wiring for improved stability:
 *   https://github.com/esp8266/Arduino/blob/esp8266/docs/ESP_improved_stability.png
 * 
 * !!!
 * NOTE: Blynk Apps currently do not support direct TCP connection.
 * Wait for new updates!
 *
 **************************************************************/

#define BLYNK_USE_DIRECT_CONNECT
//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleUserDefined.h>

// Uncomment this to set WiFi to Access Point mode
#define WIFI_MODE_AP

// Set these to your credentials.
const char ssid[]     = "Blynk-AP";    // WiFi name
const char password[] = "12345678";   // WiFi password

// Set port of the server
const int port = 8442;

// Auth token doesn't matter with direct-connect
char auth[] = "";

// Description of the dashboard
char profile[] = R"json(
  {
    "dashBoards": [
      {
        "id": 1,
        "name": "Direct connect",
        "boardType": "ESP8266",
        "widgets": [
          {
            "id": 2,
            "type": "DIGIT4_DISPLAY",
            "pinType": "VIRTUAL",
            "pin": 9,
            "x": 5,
            "y": 1,
            "frequency": 1000
          },
          {
            "id": 3,
            "type": "BUTTON",
            "pinType": "VIRTUAL",
            "pin": 1,
            "x": 2,
            "y": 1
          }
        ]
      }
    ]
  }
)json";

// Virtual handlers for our widgets...

BLYNK_READ(V9) {
  Blynk.virtualWrite(9, millis() / 1000);
}

BLYNK_WRITE(V1) {
  BLYNK_LOG("Button event");
}

// Next goes the hard stuff: connection management, etc...

WiFiServer server(port);
WiFiClient client;

// This function is used by Blynk to receive data
size_t BlynkStreamRead(void* buf, size_t len)
{
  return client.readBytes((byte*)buf, len);
}

// This function is used by Blynk to send data
size_t BlynkStreamWrite(const void* buf, size_t len)
{
  return client.write((byte*)buf, len);
}

void setup()
{
  delay(1000);
  // Setup your connection here.
  Serial.begin(9600);
  Serial.println();

  Blynk.begin(auth);
  Blynk.setProfile(profile);

#ifdef WIFI_MODE_AP
  BLYNK_LOG("Configuring WiFi access point: %s", ssid);
  WiFi.softAP(ssid, password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
#else
  BLYNK_LOG("Connecting to WiFi %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("STA IP address: ");
  Serial.println(WiFi.localIP());
#endif

  server.begin();
  BLYNK_LOG("Listening on port: %d", port);
}

void loop()
{
  client = server.available();
  // Make sure that Blynk.run() is called
  // only when the connection is established
  if (client) {
    Serial.print("Client connected from ");
    Serial.print(client.remoteIP());
    Serial.print(":");
    Serial.println(client.remotePort());

    // Wait for connection
    unsigned long start = millis();
    while (!client.connected() && (millis() - start < 1000)) {
      delay(5);
    }

    // Go!!!
    Blynk.startSession();
    while (client.connected()) {
      // Okay, handle Blynk protocol
      bool hasIncomingData = (client.available() > 0);
      // Tell Blynk if it has incoming data
      // (this allows to skip unneeded BlynkStreamRead calls)
      if (!Blynk.run(hasIncomingData)) {
        Serial.print("Error happened or disconnected.");
        break;
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}

