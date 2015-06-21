#include <WiFlyHQ.h>

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"
#define SerialWiFly     Serial1

//#include <SoftwareSerial.h>
//SoftwareSerial SerialWiFly(2, 3);

const char* host = "192.168.0.104";
uint16_t port = 8888;

WiFly wifly;

void setup(void)
{
  Serial.begin(115200);
  delay(10);
  SerialWiFly.begin(9600);
  if (!wifly.begin(&SerialWiFly, &Serial)) {
    Serial.println("Failed to start wifly");
  }

  wifly.setBaud(115200);
  SerialWiFly.begin(115200);

  /* Join wifi network if not already associated */
  if (!wifly.isAssociated()) {
    /* Setup the WiFly to connect to a wifi network */
    Serial.println("Joining network");
    wifly.setSSID(WLAN_SSID);
    wifly.setPassphrase(WLAN_PASS);
    wifly.enableDHCP();
    if (wifly.join()) {
      Serial.println("Joined wifi network");
    } else {
      Serial.println("Failed to join wifi network");
    }
  } else {
    Serial.println("Already in network");
  }

  if (wifly.isConnected()) {
    Serial.println("Old connection active. Closing");
    wifly.close();
  }

  wifly.setIpFlags(1 << 1);
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

  if (wifly.open(host, port)) {
    Serial.println(F("Connected."));
    /* Echo incoming data */
    while (wifly.isConnected()) {
      byte buf[128];
      int qty = wifly.available();
      if (qty) {
        qty = wifly.readBytes(buf, qty);
        qty = wifly.write((const uint8_t*)buf, qty);
        draw('.');
      }
    }
  } else {
    Serial.println(F("Connection failed"));
  }
  wifly.close();
}

