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
 *
 * This example shows how to use LightBlue Bean / Bean+
 * to connect your project to Blynk.
 *
 * For this example you need to install Bean board support files:
 *   https://github.com/PunchThrough/PunchThrough-BEAN-Arduino-Firmware
 *
 * NOTE: BLE support is in beta!
 *
 **************************************************************/

// WARNING: Blynk Apps do not work with the Bean directly (yet).
//#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_NO_YIELD
#include <BlynkSimpleSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Bean.enableWakeOnConnect(true);
  Blynk.begin(auth);
}

void loop()
{
  if (Bean.getConnectionState()) {
    Blynk.run();
  } else {
    // Conserve power
    Bean.setLed(0, 0, 0);
    Bean.sleep(0xFFFFFFFF);
    // Indicate connection
    Bean.setLed(0, 0, 32);

    for (int i = 0; i < 6; i++) {
      if (!Bean.getConnectionState() || Blynk.connect()) {
        break;
      }
    }
    if (Blynk.connected()) {
      Bean.setLed(0, 32, 0);
    } else {
      Bean.setLed(32, 0, 0);
    }
  }
}

BLYNK_READ(V0) {
  Blynk.virtualWrite(V0, millis() / 1000);
}

// Attach a ZeRGBa widget to the Virtual pin 2 - and control the built-in RGB led!
BLYNK_WRITE(V2) {
  Bean.setLed(
    param[0].asInt(),
    param[1].asInt(),
    param[2].asInt()
  );
}

WidgetTerminal terminal(V3);

BLYNK_WRITE(V3) {
  terminal.print("Bean got:");
  terminal.write(param.getBuffer(), param.getLength());
  terminal.println();
  terminal.flush();
}

