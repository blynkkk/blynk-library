#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11);
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>

// 1. Connect Arduino to your computer with USB cable.
// 2. Edit COMM_PORT in the script to match serial port of your Arduino:
//     for Windows:         scrips/blynk-ser.bat
//     for Linux and OSX:   scrips/blynk-ser.sh
// 3. Run the script

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

void setup()
{
  SwSerial.begin(9600);
  Blynk.begin(auth, 115200);
}

void loop()
{
  Blynk.run();
}

