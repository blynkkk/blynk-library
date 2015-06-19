#include <SPI.h>
#include <WiFly.h>

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"
#define SerialWiFly     Serial1

//#include <SoftwareSerial.h>
//SoftwareSerial SerialWiFly(2, 3);

const char* host = "192.168.0.104";
uint16_t port = 8888;

void setup(void)
{
  Serial.begin(115200);
  delay(10);

  SerialWiFly.begin(9600);
  WiFly.setUart(&SerialWiFly);
  WiFly.begin();

  Serial.println("Join....");
  if (!WiFly.join(WLAN_SSID, WLAN_PASS)) {
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }
}

void draw(char c, int qty = 1) {
  static int col = 0;
  while (qty-- > 0) {
    Serial.print(c);
    col = (col + 1) % 80;
    if (!col) {
      Serial.println();
    }
  }
}

void loop(void)
{
  delay(5000);
  Serial.print("Connecting...");

  WiFlyClient client(host, port);
  if (client.connect()) {
    Serial.println(F("Connected."));
    /* Echo incoming data */
    while (client.connected()) {
      byte buf[128];
      int qty = client.available();
      if (qty) {
        qty = client.readBytes(buf, qty);
        qty = client.write((const uint8_t*)buf, qty);
        draw('.');
      }
    }
  } else {
    Serial.println(F("Connection failed"));
  }
  client.stop();
}

