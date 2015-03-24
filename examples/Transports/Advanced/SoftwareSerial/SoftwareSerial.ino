#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SoftwareSerial.h>
#include <BlynkApiArduino.h>
#include <Adapters/BlynkSerial.h>

typedef BlynkTransportSerial<SoftwareSerial> SwSerialTransp;

SoftwareSerial mySerial(10,11);
SwSerialTransp myTransp(mySerial);
BlynkSerial<SwSerialTransp> Blynk(myTransp);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, 9600);
}

void loop()
{
  Blynk.run();
}

