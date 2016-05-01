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
 * This example code is in public domain.
 *
 **************************************************************
 * Project setup in the Blynk app:
 *   Gauge widget on V4
 *
 **************************************************************/

#include <SPI.h>
#include <Ethernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
const char auth[] = "YourAuthToken";

// Network settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Blynk cloud server
const char* host = "blynk-cloud.com";
unsigned int port = 8080;

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  delay(10);
  // Wait for serial port to connect. Needed for native USB port only
  while (!Serial) {}
  Serial.println();
  Serial.println();

  // Start the Ethernet connection
  Serial.println("Starting Ethernet...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while (true);
  }

  // Give the Ethernet shield a second to initialize
  delay(1000);
  Serial.println("Network connected");
}

void loop() {
  Serial.print("Connecting to ");
  Serial.print(host);
  Serial.print(":");
  Serial.print(port);
  Serial.print("... ");
  if (client.connect(host, port)) {
    Serial.println("OK");

    char pin[] = "V2";

    String getReq = String("GET /") + auth + "/pin/" + pin + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n" +
                    "\r\n";

    Serial.println("Sending GET request");
    client.print(getReq);
    //Serial.println(getReq);

    Serial.println("Waiting response");
    int timeout = millis() + 5000;
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    Serial.println("Reading response");
    int contentLength = 0;
    while (client.available()) {
      String line = client.readStringUntil('\n');
      line.trim();
      if (line.startsWith("Content-Length:")) {
        contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
      } else if (line.length() == 0) {
        break;
      }
    }

    // Read body
    String body;
    while (body.length() < contentLength && client.connected()) {
      while (client.available()) {
        char c = client.read();
        body += c;
      }
    }
    client.stop();
    Serial.println("Done.");

    Serial.print("Read value: ");
    Serial.println(body);

    while (true) {
      delay(0);
    }
  } else {
    Serial.println("failed");
    delay(5000);
  }
}

