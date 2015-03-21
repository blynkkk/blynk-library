/*
 * 1. Optional, but recommended.
 *    Connect additional USB-serial adapter to see the prints.
 *
 * 2. Edit auth token and upload this sketch.
 *    Be sure to select the right port (there may be multiple).
 *
 * 3. Run the script (it will redirect traffic to server):
 *      for Windows:         scripts/blynk-ser.bat
 *      for Linux and OSX:   ./scripts/blynk-ser.sh (may need to run with sudo)
 *
 *    You can specify port, baud rate, and server endpoint like this:
 *      ./blynk-ser.sh -c <serial port> -b <baud rate> -s <server address> -p <server port>
 *
 *    Run blynk-ser.sh -h for more information
 *
 *    Attention!
 *        Arduino IDE may complain with "programmer is not responding".
 *        You need to terminate script before uploading new sketch.
 *
 * 4. Start blynking! :)
 *
 */

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11);
#define BLYNK_PRINT SwSerial
#include <BlynkSimpleSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  SwSerial.begin(9600);
  Blynk.begin(auth, 9600);
}

void loop()
{
  Blynk.run();
}

