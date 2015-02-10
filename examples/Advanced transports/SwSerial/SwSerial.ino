#include <SoftwareSerial.h>
#include <BlynkSerial.h>

typedef BlynkArduinoStream<SoftwareSerial> SwSerialTransp;

SoftwareSerial mySerial(10,11);
SwSerialTransp myTransp(mySerial);
BlynkSerial<SwSerialTransp> Blynk(myTransp);

void setup()
{
  Blynk.begin("6b291640fd1c4bf69368bf555f514bb9", 9600);
}

void loop()
{
  Blynk.run();
}

