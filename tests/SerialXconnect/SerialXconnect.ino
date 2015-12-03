/*
 * This utility allows you to cross-connect
 * 2 different serials.
 *
 * This is usefull for checking wiring
 * or to convert baud rate, etc.
 */
#include <SoftwareSerial.h>
SoftwareSerial SerialSw(2, 3);

#define ser1 Serial

#define ser2 Serial1
//#define ser2 SerialSw

void setup() {
  ser1.begin(9600);
  while (!ser1);
  delay(10);
  ser2.begin(9600);
  delay(10);
}

void loop() {
  if (ser1.available()) {
    ser2.write(ser1.read());
  }
  if (ser2.available()) {
    ser1.write(ser2.read());
  }
}

