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

  This example code is in public domain.

 *************************************************************
  Project setup in the Blynk app:
    Value Display widget on V2

 *************************************************************/

#include <ESP8266WiFi.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
const char auth[] = "YourAuthToken";

// Network settings
const char ssid[] = "YourWiFi";
const char pass[] = "YourPassword";

// Blynk cloud server
const char* host = "blynk-cloud.com";
unsigned int port = 8080;

WiFiClient client;

// Start the WiFi connection
void connectNetwork()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

bool httpRequest(const String& method,
                 const String& request,
                 String&       response)
{
  Serial.print(F("Connecting to "));
  Serial.print(host);
  Serial.print(":");
  Serial.print(port);
  Serial.print("... ");
  if (client.connect(host, port)) {
    Serial.println("OK");
  } else {
    Serial.println("failed");
    return false;
  }

  client.print(method); client.println(F(" HTTP/1.1"));
  client.print(F("Host: ")); client.println(host);
  client.println(F("Connection: close"));
  if (request.length()) {
    client.println(F("Content-Type: application/json"));
    client.print(F("Content-Length: ")); client.println(request.length());
    client.println();
    client.print(request);
  } else {
    client.println();
  }

  //Serial.println("Waiting response");
  int timeout = millis() + 5000;
  while (client.available() == 0) {
    if (timeout - millis() < 0) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return false;
    }
  }

  //Serial.println("Reading response");
  int contentLength = -1;
  while (client.available()) {
    String line = client.readStringUntil('\n');
    line.trim();
    line.toLowerCase();
    if (line.startsWith("content-length:")) {
      contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
    } else if (line.length() == 0) {
      break;
    }
  }

  //Serial.println("Reading response body");
  response = "";
  response.reserve(contentLength + 1);
  while (response.length() < contentLength && client.connected()) {
    while (client.available()) {
      char c = client.read();
      response += c;
    }
  }
  client.stop();
  return true;
}

void setup()
{
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println();

  connectNetwork();
}

void loop() {
  String response;

  unsigned long value = millis();

  // Send value to the cloud
  // similar to Blynk.virtualWrite()

  Serial.print("Sending value: ");
  Serial.println(value);

  String putData = String("[\"") + value + "\"]";
  if (httpRequest(String("PUT /") + auth + "/update/V2", putData, response)) {
    if (response.length() != 0) {
      Serial.print("WARNING: ");
      Serial.println(response);
    }
  }

  // Read the value back
  // similar to Blynk.syncVirtual()

  Serial.println("Reading value");

  if (httpRequest(String("GET /") + auth + "/get/V2", "", response)) {
    Serial.print("Value from server: ");
    Serial.println(response);
  }

  // Set Property
  Serial.println("Setting property");

  if (httpRequest(String("GET /") + auth + "/update/V2?label=" + value, "", response)) {
    if (response.length() != 0) {
      Serial.print("WARNING: ");
      Serial.println(response);
    }
  }

  // For more HTTP API, see http://docs.blynkapi.apiary.io

  // Wait
  delay(30000L);
}

