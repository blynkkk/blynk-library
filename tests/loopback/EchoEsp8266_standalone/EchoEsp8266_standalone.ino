#include <ESP8266WiFi.h>

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"

IPAddress ip(192, 168, 0, 105);
uint16_t port = 8888;

void setup(void)
{
  Serial.begin(115200);
  delay(10);

  Serial.print("\nConnecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nLocal IP: ");
  Serial.println(WiFi.localIP());
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
  Serial.print(F("Connecting to "));
  Serial.println(ip);

  WiFiClient client;
  if (client.connect(ip, port)) {
    Serial.println(F("Connected."));
    /* Echo incoming data */
    while (client.connected()) {
      byte buf[128];
      int qty = client.available();
      if (qty) {
        yield();
        qty = client.readBytes((uint8_t*)buf, qty);
        yield();
        qty = client.write((const uint8_t*)buf, qty);
        draw('.');
      }
      yield();
    }
  } else {
    Serial.println(F("Connection failed"));
  }
  client.stop();

  // Delay before next connection attempt
  delay(5000);
}
