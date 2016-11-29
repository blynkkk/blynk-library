/**************************************************************
 * This is just a test of Blynk library build in Arduino IDE
 *
 *         You should NOT flash this program
 *         to your hardware or try to run it.
 *
 **************************************************************/

#if defined(ARDUINO_AVR_GEMMA) \
  || defined(ARDUINO_attiny) \
  || defined(ARDUINO_AVR_TRINKET3) \
  || defined(ARDUINO_AVR_TRINKET5) \
  || defined(ARDUINO_AVR_DIGISPARK)

  #define BLYNK_NO_INFO
  #define BLYNK_NO_BUILTIN

  #define SKIP_WRITES_TEST

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

BLYNK_WRITE(V3)
{
  test = param.asInt();
}

BLYNK_READ(V4)
{
  Blynk.virtualWrite(V10, millis(), BlynkFreeRam());

  Blynk.virtualWrite(V10, 1,   1U);
  Blynk.virtualWrite(V10, 1L,  1UL);
  Blynk.virtualWrite(V10, 1LL, 1ULL);

#ifndef SKIP_WRITES_TEST

  Blynk.virtualWrite(V10,  (int8_t)1,  (uint8_t)1);
  Blynk.virtualWrite(V10, (int16_t)1, (uint16_t)1);
  Blynk.virtualWrite(V10, (int32_t)1, (uint32_t)1);
  //Blynk.virtualWrite(V10, (int64_t)1, (uint64_t)1);

  Blynk.virtualWrite(V10, (size_t)1);

#ifndef BLYNK_NO_FLOAT
  Blynk.virtualWrite(V10, (float)1.0F);
  Blynk.virtualWrite(V10, (double)1.0);
#endif

  Blynk.virtualWrite(V10, String("Some string as String)"));
  Blynk.virtualWrite(V10, "Some string from RAM");

  Blynk.virtualWrite(V10, BLYNK_F("Some string from Flash"));

  BlynkParamAllocated param(128);
  Blynk.virtualWrite(V10, param);

  Blynk.virtualWriteBinary(V10, "buffer", 6);

#endif

}

void loop()
{
  bool hasIncomingData = (test > 0);

  if (!Blynk.run(hasIncomingData)) {

  }
}

