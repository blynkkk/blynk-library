#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>

Servo servo;

void setup()
{
  Blynk.begin("00000000000000000000000000000000");
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

