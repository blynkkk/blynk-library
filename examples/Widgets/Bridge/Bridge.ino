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
 * Control another device using Bridge widget!
 *
 * Bridge is initialized with the token of any (Blynk-enabled) device.
 * After that, use the familiar functions to control it:
 *   bridge.digitalWrite(8, HIGH)
 *   bridge.digitalWrite("A0", LOW) // <- target needs to support "Named pins"
 *   bridge.analogWrite(3, 123)
 *   bridge.virtualWrite(V1, "hello")
 *
 * WARNING :
 * For this example you'll need SimpleTimer library:
 *   https://github.com/jfturcot/SimpleTimer
 * Visit this page for more information:
 *   http://playground.arduino.cc/Code/SimpleTimer
 *
 *
 **************************************************************/

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Bridge widget on virtual pin 1
WidgetBridge bridge1(V1);

// Timer for blynking
SimpleTimer timer;

static bool value = true;
void blynkAnotherDevice() // Here we will send HIGH or LOW once per second
{
  // Send value to another device
  if (value) {
    bridge1.digitalWrite(9, HIGH);  // Digital Pin 9 on the second board will be set HIGH
    bridge1.virtualWrite(V5, 1); // Sends 1 value to BLYNK_WRITE(V5) handler on receiving side.

    /////////////////////////////////////////////////////////////////////////////////////////
    // Keep in mind that when performing virtualWrite with Bridge,
    // second board will need to process the incoming command.
    // It can be done by using this handler on the second board:
    //
    //    BLYNK_WRITE(V5){
    //    int pinData = param.asInt(); // pinData variable will store value that came via Bridge
    //    }
    //
    /////////////////////////////////////////////////////////////////////////////////////////
  } else {
    bridge1.digitalWrite(9, LOW); // Digital Pin 9 on the second board will be set LOW
    bridge1.virtualWrite(V5, 0); // Sends 0 value to BLYNK_WRITE(V5) handler on receiving side.
  }
  // Toggle value
  value = !value;
}

void setup()
{
  Blynk.begin(auth);

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  // Call blynkAnotherDevice every second
  timer.setInterval(1000L, blynkAnotherDevice);
}

BLYNK_CONNECTED() {
  bridge1.setAuthToken("OtherAuthToken"); // Place the AuthToken of the second hardware here
}

void loop()
{
  Blynk.run();
  timer.run();
}

