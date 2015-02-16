#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>

Servo servo;

void setup()
{
  Blynk.begin("bfedeff2b5a1445eb6e0519a4c965b8c");
  servo.attach(9);
}

BLYNK_ON_WRITE(2)
{
  servo.write(param[0].asInt());
}

BLYNK_ON_READ(3)
{
  Blynk.virtualWrite(3, millis());
}

void loop()
{
  Blynk.run();
}

