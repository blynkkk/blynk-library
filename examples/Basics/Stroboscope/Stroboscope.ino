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
 * This is a simple stroboscope.
 * You can turn it on and of using a button,
 * and control frequency with a slider.
 *
 * For this example you need SimpleTimer library:
 *   https://github.com/jfturcot/SimpleTimer
 * Visit this page for more information:
 *   http://playground.arduino.cc/Code/SimpleTimer
 *
 * App dashboard setup:
 *   Button widget (Switch) on V1
 *   Slider widget (100...1000) on V2
 *
 **************************************************************/

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

#define LED_PIN 9

SimpleTimer timer;
int t1;

void setup()
{
  // Configure Blynk
  Serial.begin(9600);
  Blynk.begin(auth);

  // Configure LED and timer
  pinMode(LED_PIN, OUTPUT);
  t1 = timer.setInterval(500, ledBlynk);
  timer.disable(t1);
}

// Enable/disable blinking using virt pin 1
BLYNK_WRITE(1)
{
  if (param[0].asInt()) {
    timer.enable(t1);
  } else {
    timer.disable(t1);
    digitalWrite(LED_PIN, LOW);
  }
}

// Change blink interval using virtual pin 2
BLYNK_WRITE(2)
{
  int interval = param[0].asInt();
  boolean wasEnabled = timer.isEnabled(t1);
  timer.deleteTimer(t1);
  t1 = timer.setInterval(interval, ledBlynk);
  if (!wasEnabled) {
    timer.disable(t1);
  }
}

//  Toggle LED
void ledBlynk()
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void loop()
{
  Blynk.run();
  timer.run();
}

