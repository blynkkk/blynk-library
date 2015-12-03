
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

