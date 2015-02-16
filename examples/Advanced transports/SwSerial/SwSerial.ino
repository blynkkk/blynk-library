#include <SoftwareSerial.h>
#include <BlynkSerial.h>

typedef BlynkTransportSerial<SoftwareSerial> SwSerialTransp;

SoftwareSerial mySerial(10,11);
SwSerialTransp myTransp(mySerial);
BlynkSerial<SwSerialTransp> Blynk(myTransp);

void setup()
{
  Blynk.begin("bfedeff2b5a1445eb6e0519a4c965b8c", 9600);
}

void loop()
{
  Blynk.run();
}

