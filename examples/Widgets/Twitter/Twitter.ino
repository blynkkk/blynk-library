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
 * Simple tweet example
 *
 * App dashboard setup:
 *   Twitter widget (connect it to your Twitter account!)
 *
 **************************************************************/
//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

SimpleTimer timer;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);
  while (!Blynk.connect()) {
    // Wait until connected
  }

  // Tweet immediately on startup
  tweetUptime();
  // Setup a function to be called every minute
  timer.setInterval(60000L, tweetUptime);
}

void tweetUptime()
{
  long uptime = millis() / 1000;
  BLYNK_LOG("Tweet ;)");

  // Actually send the message.
  // Note:
  //   We allow 1 tweet per minute for now.
  //   Twitter doesn't allow identical subsequent messages.
  Blynk.tweet(String("Running for ") + uptime + " seconds.");
}

void loop()
{
  Blynk.run();
  timer.run();
}

