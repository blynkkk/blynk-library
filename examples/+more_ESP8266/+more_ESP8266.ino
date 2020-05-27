/*************************************************************
  This is a DEMO. You can use it only for development and testing.
  You should open Setting.h and modify General options.

  If you would like to add these features to your product,
  please contact Blynk for Businesses:

                   http://www.blynk.io/

 *************************************************************/

#define USE_SPARKFUN_BLYNK_BOARD // Uncomment the board you are using
//#define USE_NODE_MCU_BOARD        // Comment out the boards you are not using
//#define USE_WITTY_CLOUD_BOARD
//#define USE_CUSTOM_BOARD          // For all other ESP8266-based boards -
// see "Custom board configuration" in Settings.h

#define APP_DEBUG // Comment this out to disable debug prints

#define BLYNK_SSL_USE_LETSENCRYPT // Comment this out, if using public Blynk Cloud

#define BLYNK_PRINT Serial

#include "BlynkProvisioning.h"

BlynkTimer timer;

void setup()
{
  delay(100);
  Serial.begin(115200);

  BlynkProvisioning.begin();
  timer.setInterval(20000L, temperature);
}

void loop()
{
  // This handles the network and cloud connection
  BlynkProvisioning.run();
  timer.run();
}

void temperature()
{
  int temperature = random(-100, 101);
  Blynk.virtualWrite(V2, temperature);
  Serial.print("Temperature: ");
  Serial.println(temperature);

  if (temperature > 90)
  {
    Blynk.logEvent("temperaturecritical");
  }
  else if (temperature > 70)
  {
    Blynk.logEvent("temperaturewarning");
  }
  else if (temperature > 50)
  {
    Blynk.logEvent("temperature_is_ok");
  }
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  Serial.print("Button: ");
  Serial.println(pinValue);
}
