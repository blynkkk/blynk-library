/*
 * Rotate a servo using a slider!
 *
 * App dashboard setup:
 *   Slider (0...180) on V3
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

WidgetLCD lcd(1);

void setup()
{
  Blynk.begin(auth);
  lcd.clear();
  lcd.print(0, 1, "Wow! :)");
}

void loop()
{
  Blynk.run();
}

