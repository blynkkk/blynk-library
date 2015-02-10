#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <VirtualSerial.h>

VirtualSerial BlynkSerial(1, onReceive);

void setup()
{
  Blynk.begin("6b291640fd1c4bf69368bf555f514bb9");
  Blynk.onConnect(onConnect);
}

void onConnect()
{
  BlynkSerial.println("Hello there ;)");
}

void onReceive(const uint8_t* buff, size_t len)
{
  BlynkSerial.print("I got:");
  BlynkSerial.write(buff, len);
}

void loop()
{
  Blynk.run();
}

