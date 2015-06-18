#include <SPI.h>
#include <Ethernet.h>

IPAddress ip(192, 168, 0, 105);
uint16_t port = 8888;

EthernetClient client;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup(void)
{
  Serial.begin(115200);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
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
      for (int i = 0; i < qty; ++i) {
        client.write(buf[i]);
        draw('.');
      }
    }
  } else {
    Serial.println(F("Connection failed"));
  }
  client.stop();

  // Delay before next connection attempt
  delay(5000);
}

