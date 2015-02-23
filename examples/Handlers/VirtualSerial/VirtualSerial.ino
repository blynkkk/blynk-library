#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <VirtualSerial.h>

VirtualSerial BlynkSerial(1);

void setup()
{
  Blynk.begin("00000000000000000000000000000000");
}

BLYNK_ON_WRITE(1)
{
  BlynkSerial.println("I got:");
  BlynkSerial.write(param.getBuffer(), param.getLength());
}

void loop()
{
  Blynk.run();
}

