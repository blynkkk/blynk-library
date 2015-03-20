/*
 * This is an example of how to turn the LED Widget ON and OFF with a physical button (on your breadboard). 
 *
 * See http://arduino.cc/en/tutorial/button for the instructions on how to set up a button on Arduino
 * 
 * Step 1: Add an LED widget to your Project inside the App. In the LED Widget settings - set Input PIN to Virtual 1 
 * Step 2: Get the AuthToken number in the App and paste it to this sketch
 * Step 3: Upload this sketch to the board, and press Play button in the App
 *
 * Happy Blynking!
 * 
 * If you need help, visit:
 * blynk.cc
 * community.blynk.cc
 *
 */

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>

void setup()
{
  Serial.begin(9600);
  Blynk.begin("YourAuthToken"); // You can get Auth Token for your Project in the Blynk App. Go to the Project Settings (nut icon)

  // Connect button to pin 2 on your Arduino. Make pin 2 default HIGH, and attach INT to our handler !!!!!!!!!(explain what handler is......)!!!!!!!!!!
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, checkPin, CHANGE);
}

void checkPin()
{
  // Invert state, since button is "Active LOW"
  int state = !digitalRead(2);
  Blynk.virtualWrite(1, state);
}

void loop()
{
  Blynk.run();
}

