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
 * This example shows how a value is periodically pushed
 * from Arduino to the phone App.
 *
 * For this example you need to have SimpleTimer library:
 *   https://github.com/jfturcot/SimpleTimer
 * Visit this page for more information:
 *   http://playground.arduino.cc/Code/SimpleTimer
 *
 * App dashboard setup:
 *   Value Display widget on V5
 *
 **************************************************************/

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h> //Download Simple Timer library here: https://github.com/jfturcot/SimpleTimer

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

SimpleTimer timer;

void setup()
{
  Blynk.begin(auth);

  // Setup function to be called each 1000 milliseconds
  timer.setInterval(1000, sendUptime);
}

// A function that sends how many seconds Arduino is running
void sendUptime()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second!
  Blynk.virtualWrite(5, millis()/1000);
}

void loop()
{
  Blynk.run();
  timer.run();
}

