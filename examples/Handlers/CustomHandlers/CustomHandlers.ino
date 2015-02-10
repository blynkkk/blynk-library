#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <Servo.h>

Servo servo;

void setup()
{
  Blynk.begin("6b291640fd1c4bf69368bf555f514bb9");
  servo.attach(9);
}

BLYNK_ON_WRITE(V2)
{
  servo.write(param[0].asInt());
}

BLYNK_ON_READ(V3)
{
  Blynk.virtualWrite(V3, millis());
}

void loop()
{
  Blynk.run();
}

