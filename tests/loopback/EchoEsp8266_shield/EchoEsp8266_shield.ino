#include "ESP8266.h"

#define WLAN_SSID   "ssid"
#define WLAN_PASS   "pass"
#define HOST_NAME   "192.168.0.105"
#define HOST_PORT   (8888)
#define EspSerial   Serial1

// This can be a SoftwareSerial object
// (remember to also edit ESP8266.h):
//#include <SoftwareSerial.h>
//SoftwareSerial SwSerial(2, 3); // RX, TX
//#define EspSerial SwSerial

ESP8266 wifi(EspSerial);

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

static void onData(uint8_t mux_id, uint32_t len, void* ptr)
{
  byte buff[128];
  if (len > sizeof(buff)) {
    draw('#', 1);
  }
  len = Serial1.readBytes(buff, len);
  wifi.send(buff, len);
  draw('.');
}

void setup(void)
{
  Serial.begin(115200);
  while (!Serial) {}

  EspSerial.begin(115200);  // Set ESP8266 baud rate
  while (!EspSerial) {}

  wifi.setOnData(onData, NULL);

  if (!wifi.setEcho(0)) {
    Serial.println("Disable echo fail.");
  }

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP(WLAN_SSID, WLAN_PASS)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP:");
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }

  Serial.print("setup end\r\n");
}

void loop(void)
{
  delay(10000);
  Serial.println(F("Connecting..."));
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
    byte buffer[128] = {0};
    Serial.println(F("Connected."));
    unsigned long start = millis();
    while (millis() - start < 10000) {
      wifi.run();
    }
    Serial.println(F("Closing..."));
    if (!wifi.releaseTCP()) {
      Serial.println(F("Release failed"));
    }
  } else {
    Serial.println(F("Connection failed"));
  }
}
