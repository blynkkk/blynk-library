/*
 * This is a simple stroboscope.
 * You can turn it on and of using a button,
 * and control frequency with a slider.
 * 
 * App dashboard setup:
 *   Button (Switch) on V1
 *   Slider (100...1000) on V2
 * 
 * For this example you need to have SimpleTimer library:
 *   https://github.com/jfturcot/SimpleTimer
 * Visit this page for more information:
 *   http://playground.arduino.cc/Code/SimpleTimer
 */

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// Auth token you get from App
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

