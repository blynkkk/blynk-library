#include <ESP8266WiFi.h>

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"

IPAddress ip(192,168,0,105);
uint16_t port = 8080;

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

void draw(char c) {
  Serial.print(c);
  static int col = 0;
  col = (col+1) % 80;
  if (!col) {
    Serial.println();
  }
}

void loop(void)
{
  Serial.print("Connecting to ");
  Serial.println(ip);

  WiFiClient client;
  if (client.connect(ip, port)) {
    Serial.println(F("Connected."));
    /* Echo incoming data */
    while (client.connected()) {
      char buf[128];
      int qty = 0;
      while (client.available()) {
        buf[qty] = client.read();
        draw(buf[qty]);
        qty++;
      }
      for (int i=0; i<qty; ++i) {
        client.write(buf[i]);
        draw('.');
        delay(10); // Allow esp8266 housekeeping
      }
      delay(10); // Allow esp8266 housekeeping
    }
  } else {
    Serial.println(F("Connection failed"));
  }
  client.stop();

  // Delay before next connection attempt
  delay(5000);
}

