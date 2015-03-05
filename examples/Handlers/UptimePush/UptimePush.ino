/*
 * This example shows how a value is periodically pushed
 * from Arduino to the phone App.
 * 
 * The dashboard:
 *   Value Display on V5
 * 
 * For this example you need to have SimpleTimer library:
 *   https://github.com/jfturcot/SimpleTimer
 * Visit this page for more information:
 *   http://playground.arduino.cc/Code/SimpleTimer
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// Auth token you get from App
char auth[] = "00000000000000000000000000000000";

SimpleTimer timer;

void setup()
{
  Blynk.begin(auth);
  timer.setInterval(1000, sendUptime);
}

// A function to be executed periodically
void sendUptime()
{
  Blynk.virtualWrite(5, millis()/1000);
}

void loop()
{
  Blynk.run();
  timer.run();
}

