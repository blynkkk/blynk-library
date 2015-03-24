/*
 * This example shows how a value is periodically pushed
 * from Arduino to the phone App.
 *
 * App dashboard setup:
 *   Value Display on V5
 *
 * For this example you need to have SimpleTimer library:
 *   https://github.com/jfturcot/SimpleTimer
 * Visit this page for more information:
 *   http://playground.arduino.cc/Code/SimpleTimer
 */

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

SimpleTimer timer;

void setup()
{
  Blynk.begin(auth);

  // Setup function to be called each 1000 milliseconds
  timer.setInterval(1000, sendUptime);
}

// A function that sends how many seconds Arduino is running
void sendUptime()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second!
  Blynk.virtualWrite(5, millis()/1000);
}

void loop()
{
  Blynk.run();
  timer.run();
}

