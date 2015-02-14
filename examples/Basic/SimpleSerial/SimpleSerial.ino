#include <BlynkSimpleSerial.h>

// Connect Arduino to your computer with USB cable.
// Specify serial port of Arduino in the script and run it:
//    Windows:         blynk-ser.bat
//    Linux and OSX:   blynk-ser.sh

// Auth token you get from App
char auth[] = "6b291640fd1c4bf69368bf555f514bb9";

void setup()
{
  Blynk.begin(auth);
  // Default baudrate is 115200. You could specify it like this:
  //Blynk.begin(auth, 9600);
}

void loop()
{
  Blynk.run();
}

