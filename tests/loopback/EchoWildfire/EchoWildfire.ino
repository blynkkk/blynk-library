#include <WildFire_CC3000.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <WildFire.h>

WildFire wildfire;
WildFire_CC3000 cc3000;

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"
#define WLAN_SECURITY   WLAN_SEC_WPA2

uint32_t ip = cc3000.IP2U32(192, 168, 0, 105);
uint16_t port = 8888;

void setup(void)
{
  wildfire.begin();

  Serial.begin(115200);
  Serial.print("\nFree RAM: "); Serial.println(getFreeRam(), DEC);

  /* Initialise the module */
  Serial.println(F("Initializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while (1);
  }

  Serial.print(F("Attempting to connect to ")); Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while (1);
  }

  Serial.println(F("Connected!"));

  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }

  /* Display the IP address DNS, Gateway, etc. */
  while (!displayConnectionDetails()) {
    delay(1000);
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
  Serial.print(F("Connecting to ")); cc3000.printIPdotsRev(ip);
  Serial.println();

  WildFire_CC3000_Client client = cc3000.connectTCP(ip, port);
  if (client.connected()) {
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
    client.close();
  } else {
    Serial.println(F("Connection failed"));
  }

  // Delay before next connection attempt
  delay(5000);
}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

