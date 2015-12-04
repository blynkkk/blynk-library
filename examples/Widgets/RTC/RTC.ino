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
 * Blynk can provide your device with time data, like an RTC.
 * Please note that the accuracy of this method is up to several seconds.
 *
 * App dashboard setup:
 *   RTC widget on V5
 *
 * WARNING :
 * For this example you'll need SimpleTimer library:
 *   https://github.com/jfturcot/SimpleTimer
 *
 * And also this Time keeping library:
 *   https://github.com/PaulStoffregen/Time
 *
 * This code is based on an example from the Time library:
 *   https://github.com/PaulStoffregen/Time/blob/master/examples/TimeSerial/TimeSerial.ino
 *
 **************************************************************/

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>
#include <Time.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

SimpleTimer timer;

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  // Set function to call when time sync required
  setSyncProvider(requestTimeSync);

  // Display digital clock every 10 seconds
  timer.setInterval(10000L, clockDisplay);
}

BLYNK_WRITE(V5)
{
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  unsigned long blynkTime = param.asLong();

  if ( blynkTime >= DEFAULT_TIME) { // Check the integer is a valid time (greater than Jan 1 2013)
    setTime(blynkTime);             // Sync Time library clock to the value received from Blynk
    BLYNK_LOG("Time sync: OK");
  }
}

// This is called by Time library when it needs time sync
time_t requestTimeSync()
{
  Blynk.syncVirtual(V5); // Request RTC widget (V5) update from the server
  return 0;              // Tell the Time library that we'll set it later
}

// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details
  BLYNK_LOG("Current time: %02d:%02d:%02d %02d %02d %d",
            hour(), minute(), second(),
            day(), month(), year()
           );
}

void loop()
{
  Blynk.run();
  timer.run();
}

