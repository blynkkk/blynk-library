#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SoftwareSerial.h>
#include <BlynkSerial.h>

typedef BlynkTransportSerial<SoftwareSerial> SwSerialTransp;

SoftwareSerial mySerial(10,11);
SwSerialTransp myTransp(mySerial);
BlynkSerial<SwSerialTransp> Blynk(myTransp);

void setup()
{
  Serial.begin(9600);
  Blynk.begin("00000000000000000000000000000000", 9600);
}

void loop()
{
  Blynk.run();
}

