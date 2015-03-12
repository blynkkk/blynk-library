/*
 * Rotate a servo using a slider!
 *
 * The dashboard:
 *   Slider (0...180) on V3
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <WidgetLCD.h>

WidgetLCD lcd(1);

void setup()
{
  Blynk.begin("00000000000000000000000000000000");
  lcd.clear();
  lcd.print(0, 1, "Wow! :)");
}

void loop()
{
  Blynk.run();
}

