#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <VirtualSerial.h>

VirtualSerial BlynkSerial(1);

void setup()
{
  Blynk.begin("bfedeff2b5a1445eb6e0519a4c965b8c");
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

