/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use AirBoard + RN-XV WiFly module
  to connect your project to Blynk.

  For this example you need WiFlyHQ library:
    https://github.com/harlequin-tech/WiFlyHQ

  Note: Be sure to read this: http://www.theairboard.cc/quick-start/
        WiFly firmware version 4.41 or later is preferred.

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(10, 11); // RX, TX

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT DebugSerial


#include <TheAirBoard.h>
#include <WiFlyHQ.h>
#include <BlynkSimpleWiFly.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define WiFlySerial Serial
WiFly wifly;

void setup()
{
  // Debug console
  DebugSerial.begin(9600);

  delay(10);
  WiFlySerial.begin(9600);  // Set your RN-XV baud rate
  delay(10);

  // Bind WiFly driver to the serial
  if (!wifly.begin(&WiFlySerial)) {
    BLYNK_FATAL("Failed to start wifly");
  }

  // You can try increasing baud rate:
  //wifly.setBaud(115200);
  //WiFlySerial.begin(115200);

  Blynk.begin(auth, wifly, ssid, pass);
}

void loop()
{
  Blynk.run();
}

