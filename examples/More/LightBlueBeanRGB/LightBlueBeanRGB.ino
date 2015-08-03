
// cause this is not defined for some reason...
void yield() {}

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

    for (int i=0; i<6; i++) {
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


double hue = 0, sat = 1, val = 1;

void updateHSV() {
  byte rgb[3];
  HsvToRgb(hue, sat, val, rgb);
  Bean.setLed(rgb[0], rgb[1], rgb[2]);
}

// Attach a Slider widget to the Virtual pin 2 - and control the built-in RGB led!
BLYNK_WRITE(V2) {
  hue = param.asDouble()/255;
  updateHSV();
}

BLYNK_WRITE(V3) {
  sat = param.asDouble()/255;
  updateHSV();
}

BLYNK_WRITE(V4) {
  val = param.asDouble()/255;
  updateHSV();
}



WidgetTerminal terminal(1);

BLYNK_WRITE(V1) {
  terminal.print("I got:");
  terminal.write(param.getBuffer(), param.getLength());
  terminal.println();
  terminal.flush();
}

void HsvToRgb(double h, double s, double v, byte rgb[]) {
    double r, g, b;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}
