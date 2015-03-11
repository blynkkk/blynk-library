/*
 * Browse SD card from phone App
 * 
 * The dashboard:
 *   SD widget on V1
 */
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <SD.h>
#include <BlynkSimpleEthernet.h>
#include <WidgetSD.h>

WidgetSD sd;

void setup()
{
  Blynk.begin("00000000000000000000000000000000");
  
  // Pin 10 (or 53 on Mega) should stay output - read SD examples for details
  pinMode(10, OUTPUT);
  
  // Arduino Ethernet shield: pin 4
  // Adafruit SD shields and modules: pin 10
  // Sparkfun SD shield: pin 8
  const int chipSelect = 4;
  if (!SD.begin(chipSelect)) {
    Serial.println("SD init failed!");
    return;
  }
}

BLYNK_ATTACH_WIDGET(sd, 1)

void loop()
{
  Blynk.run();
}

