/*
 * This utility allows you to cross-connect
 * 2 different serials.
 *
 * This is usefull for checking wiring
 * or to convert baud rate, etc.
 */

#define ser1 Serial

#if defined(HAVE_HWSERIAL1)
  #define ser2 Serial1
#else
  #warning Using Software Serial!
  #include <SoftwareSerial.h>
  SoftwareSerial SerialSw(2, 3);
  #define ser2 SerialSw
#endif

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

