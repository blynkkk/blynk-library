/**************************************************************
 * This is just a test of Blynk library memory footprint.
 * It doesn't use serial, wifi, ethernet or any other library.
 * It uses a volatile variable to simulate server stream.
 *
 *         You should NOT flash this program
 *         to your hardware or try to run it.
 *
 **************************************************************/

//#define ENABLE_MINIMIZATION
#define ENABLE_HANDLERS

#ifdef ENABLE_MINIMIZATION
  #define BLYNK_NO_BUILTIN
  #define BLYNK_NO_INFO
  #define BLYNK_NO_FLOAT
#endif

#include <BlynkSimpleUserDefined.h>

char auth[] = "12345678901234567890123456789012";

volatile uint8_t test;

// This function is used by Blynk to receive data
size_t BlynkStreamRead(void* buf, size_t len)
{
  uint8_t* byte_buff = (uint8_t*)buf;
  size_t res = len;
  while (len--) {
    *byte_buff++ = test;
  }
  return res;
}

// This function is used by Blynk to send data
size_t BlynkStreamWrite(const void* buf, size_t len)
{
  uint8_t* byte_buff = (uint8_t*)buf;
  size_t res = len;
  while (len--) {
    test = *byte_buff++;
  }
  return res;
}

void setup()
{
  Blynk.begin(auth);
  Blynk.connect();
}

#ifdef ENABLE_HANDLERS

BLYNK_CONNECTED()
{
  test = 1;
}

BLYNK_DISCONNECTED()
{
  test = 2;
}

BLYNK_WRITE(V3)
{
  test = param.asInt();
}

BLYNK_READ(V4)
{
  Blynk.virtualWrite(V4, millis(), BlynkFreeRam());
}

#endif /* ENABLE_HANDLERS */

void loop()
{
  bool hasIncomingData = (test > 0);

  if (!Blynk.run(hasIncomingData)) {

  }
}

