#include <BlynkSimpleSerial.h>

// Connect Arduino to your computer with USB cable.
// Run an app to relay Arduino <-> server connection.
//    Windows:       blynk-ser.bat
//    Linux and OSX: blynk-ser.sh
// Edit the script to specify serial port of Arduino and run it

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

