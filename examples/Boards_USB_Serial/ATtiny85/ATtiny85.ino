/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social groups:              http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * This example shows how to use a tiny ATtiny85
 * to connect your project to Blynk.
 *
 * Note: This requires ATtiny support package:
 *   https://github.com/damellis/attiny
 *
 * Be sure to check ordinary Serial example first!!!
 *
 *                  +-\/-+
 * A0|6 (D 5) PB5  1|    |8  Vcc
 * A3|9 (D 3) PB3  2|    |7  PB2 (D 2) A1|7
 * A2|8 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
 *            GND  4|    |5  PB0 (D 0) pwm0
 *                  +----+
 *
 **************************************************************
 *            USB HOWTO: http://tiny.cc/BlynkUSB
 **************************************************************/
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(1, 2); // RX, TX
SoftwareSerial SwSerial(3, 4);

//#define BLYNK_DEBUG
#define BLYNK_PRINT DebugSerial    // Comment this out to disable prints and save space
#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

//#include <SoftwareServo.h>
//SoftwareServo servo1;

void setup()
{
  // Debug console
  DebugSerial.begin(9600);

  // Blynk will work through SoftwareSerial
  SwSerial.begin(9600);
  Blynk.begin(auth, SwSerial);

  // Sometimes you need to calibrate your ATtiny timer
  //OSCCAL = 175;
}

void loop()
{
  Blynk.run();
}

