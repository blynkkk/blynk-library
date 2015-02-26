#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// For this example you need to have SimpleTimer library:
// https://github.com/jfturcot/SimpleTimer

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

SimpleTimer timer;

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth);
  pinMode(9, OUTPUT);
  timer.setInterval(500, repeatMe);
}

// a function to be executed periodically
void repeatMe()
{
  digitalWrite(9, !digitalRead(9));
}

void loop()
{
  Blynk.run();
  timer.run();
}

