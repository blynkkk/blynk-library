/*
 * Browse SD card from phone App
 *
 * App dashboard setup:
 *   SD widget on V1
 */
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <SD.h>
#include <BlynkSimpleEthernet.h>

WidgetSD sd;

BLYNK_ATTACH_WIDGET(sd, 1)

void setup()
{
  Serial.begin(9600);

  // Arduino Ethernet shield: pin 4
  // Adafruit SD shields and modules: pin 10
  // Sparkfun SD shield: pin 8
  const int sdChipSelect = 4;

  // Pin 10 (or 53 on Mega) should stay output - read SD examples for details
  pinMode(SS, OUTPUT);

  // Disable the W5100 and init SD
  digitalWrite(10, HIGH);
  if (!SD.begin(sdChipSelect)) {
    BLYNK_FATAL("SD init failed!");
  }

  // Init Ethernet & Blynk
  Blynk.begin("YourAuthToken");
}

void loop()
{
  Blynk.run();
}

