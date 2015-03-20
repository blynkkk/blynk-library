

//   Please rename sketch to SoftwareSerial.ino


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
  Blynk.begin("YourAuthToken", 9600); // You can get Auth Token in the Blynk App. Go to the Project Settings (nut icon). 
}

void loop()
{
  Blynk.run();
}

