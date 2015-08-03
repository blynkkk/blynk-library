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
 * Simple email example
 *
 * App dashboard setup:
 *   Email widget
 *
 * Connect a button to pin 2 and GND...
 * Pressing this button will also email a message! ;)
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

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  // Send email immediately on startup
  Blynk.email("my_email@example.com", "Title", "Body");

  // Setup a function to be called every 10 minutes
  timer.setInterval(10L * 60000L, emailUptime);

  // Setup email button on pin 2
  pinMode(2, INPUT_PULLUP);
  // Attach pin 2 interrupt to our handler
  attachInterrupt(digitalPinToInterrupt(2), emailOnButtonPress, CHANGE);
}

void emailUptime()
{
  long uptime = millis() / 60000L;
  BLYNK_LOG("Sending email every 10 minutes ;)");

  // Actually send the message.
  // Note:
  //   We allow 1 email per minute for now.
  Blynk.email("my_email@example.com", "Title", "Body");
}

void emailOnButtonPress()
{
  // Invert state, since button is "Active LOW"
  int isButtonPressed = !digitalRead(2);
  if (isButtonPressed) {
    BLYNK_LOG("Button is pressed.");

    Blynk.email("my_email@example.com", "Title", "Body");
  }
}

void loop()
{
  Blynk.run();
  timer.run();
}

