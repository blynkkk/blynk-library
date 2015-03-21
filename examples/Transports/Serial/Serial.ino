/*
 * Hey! It is recommended to start with "Serial with prints" example.
 * You will find a detailed explanation how to use Blynk over serial,
 * and also enables prints for you to see if everything goes OK.
 */

#include <BlynkSimpleSerial.h>

char auth[] = "YourAuthToken";

void setup()
{
  Blynk.begin(auth);
  // Default baud rate is 9600. You could specify it like this:
  //Blynk.begin(auth, 57600);
}

void loop()
{
  Blynk.run();
}

