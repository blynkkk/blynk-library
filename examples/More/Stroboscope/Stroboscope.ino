/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This is a simple stroboscope.
  You can turn it on and of using a button,
  and control frequency with a slider.

  App dashboard setup:
    Button widget (Switch) on V1
    Slider widget (100...1000) on V2
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

#define LED_PIN 4

BlynkTimer timer;
BlynkTimer::Handle t1;

// Toggle LED
void ledBlynk()
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

// Enable/disable blinking using virtual pin 1
BLYNK_WRITE(V1)
{
  if (param.asInt()) {
    t1.enable();
  } else {
    t1.disable();
    digitalWrite(LED_PIN, LOW);
  }
}

// Change blink interval using virtual pin 2
BLYNK_WRITE(V2)
{
  long interval = param.asLong();
  t1.changeInterval(interval);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN);

  // Configure LED and timer
  pinMode(LED_PIN, OUTPUT);
  t1 = timer.setInterval(500L, ledBlynk);
  t1.disable();
}

void loop()
{
  Blynk.run();
  timer.run();
}

