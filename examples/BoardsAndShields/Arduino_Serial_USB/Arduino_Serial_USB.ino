/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example shows how to use ordinary Arduino Serial
 * to connect your project to Blynk.
 * Feel free to apply it to any other example. It's simple!
 *
 * 1. Optional, but recommended.
 *    Connect additional USB-serial adapter to see the prints.
 *
 * 2. Edit auth token and upload this sketch.
 *
 * 3. Run the script (script located in "scripts" folder of library root,
 *    e.g. 'blynk-library/scripts') for redirecting traffic to server:
 *
 *      for Windows:
 *                     1. Open cmd.exe
 *                     2. write : (your way to blynk-ser.bat folder) example: "cd C:\blynk-library-0.3.1\blynk-library-0.3.1\scripts"
 *                     3. write : "blynk-ser.bat -c COM4" (where COM4 is port with your Arduino)
 *                     4. And press "Enter" , press "Enter" and press "Enter"
 *
 *      for Linux and OSX:
 *
 *                    ./blynk-ser.sh (may need to run with sudo)
 *
 *    You can specify port, baud rate, and server endpoint like this:
 *      ./blynk-ser.sh -c <serial port> -b <baud rate> -s <server address> -p <server port>
 *
 *    For instance :
 *      ./blynk-ser.sh -c /dev/ttyACM0 -b 9600 -s cloud.blynk.cc -p 8442
 *
 *    Run blynk-ser.sh -h for more information
 *
 *    Be sure to select the right serial port (there may be multiple).
 *
 *    Attention!
 *        Arduino IDE may complain with "programmer is not responding".
 *        You need to terminate script before uploading new sketch.
 *
 * 4. Start blynking! :)
 *
 **************************************************************/

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(2, 3); // RX, TX
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  SwSerial.begin(9600);
  Blynk.begin(auth);
  // Default baud rate is 9600. You could specify it like this:
  //Blynk.begin(auth, 57600);
}

void loop()
{
  Blynk.run();
}

