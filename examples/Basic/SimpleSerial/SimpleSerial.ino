#include <BlynkSimpleSerial.h>

// 1. Connect Arduino to your computer with USB cable.
// 2. Edit COMM_PORT in the script to match serial port of your Arduino:
//     for Windows:         scrips/blynk-ser.bat
//     for Linux and OSX:   scrips/blynk-ser.sh
// 3. Run the script

// Auth token you get from App
char auth[] = "bfedeff2b5a1445eb6e0519a4c965b8c";

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

