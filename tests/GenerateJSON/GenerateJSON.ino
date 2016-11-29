#include <Blynk.h>
#include <Blynk/BlynkDetectDevice.h>

/*
 * Pins Quantity
 */

#if defined(NUM_DIGITAL_PINS)
  #define BOARD_DIGITAL_MAX int(NUM_DIGITAL_PINS)
#elif defined(PINS_COUNT)
  #define BOARD_DIGITAL_MAX int(PINS_COUNT)
#else
  #warning "BOARD_DIGITAL_MAX not detected"
  #define BOARD_DIGITAL_MAX 32
#endif

#if defined(NUM_ANALOG_INPUTS)
  #define BOARD_ANALOG_IN_MAX int(NUM_ANALOG_INPUTS)
#else
  #warning "BOARD_ANALOG_IN_MAX not detected"
  #define BOARD_ANALOG_IN_MAX 0
#endif

#if defined(BLYNK_USE_128_VPINS)
  #define BOARD_VIRTUAL_MAX 127
#else
  #define BOARD_VIRTUAL_MAX 31
#endif

/*
 * Pins Functions
 */

#ifndef digitalPinHasPWM
  #warning "No digitalPinHasPWM"
  #define digitalPinHasPWM(x) false
#endif

#if !defined(analogInputToDigitalPin)
  #warning "No analogInputToDigitalPin"
  #define analogInputToDigitalPin(x) -1
#endif

/*
 * Pins Ranges
 */

#if !defined(BOARD_PWM_MAX)
  #if   defined(PWMRANGE)
    #define BOARD_PWM_MAX    PWMRANGE
  #elif defined(PWM_RESOLUTION)
    #define BOARD_PWM_MAX    ((2^(PWM_RESOLUTION))-1)
  #else
    #warning "Cannot detect BOARD_PWM_MAX"
    #define BOARD_PWM_MAX    255
  #endif
#endif

#if !defined(BOARD_ANALOG_MAX)
  #if   defined(ADC_RESOLUTION)
    #define BOARD_ANALOG_MAX    ((2^(ADC_RESOLUTION))-1)
  #else
    #warning "Cannot detect BOARD_ANALOG_MAX"
    #define BOARD_ANALOG_MAX    1023
  #endif
#endif

#if defined(clockCyclesPerMicrosecond)
  #define BOARD_INFO_MHZ clockCyclesPerMicrosecond()
#elif defined(F_CPU)
  #define BOARD_INFO_MHZ ((F_CPU)/1000000UL)
#endif


struct Ser {
  template<typename T, typename... Args>
  void print(T last) {
    Serial.print(last);
  }

  template<typename T, typename... Args>
  void print(T head, Args... tail) {
    Serial.print(head);
    print(tail...);
  }
} ser;

const char* JS[] = {
  "\n"
  "{\n"
  "    ",
  "\"map\": {\n"
  "        \"digital\": {\n"
  "            \"pins\": {\n"
  "                 ", "\n"
  "            },\n"
  "            \"ops\": [ \"dr\", \"dw\" ]\n"
  "        },\n"
  "        \"analog\": {\n"
  "            \"pins\": {\n"
  "                 ", "\n"
  "            },\n"
  "            \"ops\": [ \"dr\", \"dw\", \"ar\" ],\n"
  "            \"arRange\": [ 0, ", " ]\n"
  "        },\n"
  "        \"pwm\": {\n"
  "            \"pins\": [\n"
  "                ", "\n"
  "            ],\n"
  "            \"ops\": [ \"aw\" ],\n"
  "            \"awRange\": [ 0, ", " ]\n"
  "        },\n"
  "        \"virtual\":  {\n"
  "            \"pinsRange\": [ 0, ", " ],\n"
  "            \"ops\": [ \"vr\", \"vw\" ]\n"
  "        }\n"
  "    }\n"
  "}\n"
};



void setup() {
  Serial.begin(9600);
  delay(10);
}

void loop() {
  ser.print(JS[0]);
  ser.print("\"name\": \"", BLYNK_INFO_DEVICE, "\",\n    ");
#ifdef BLYNK_INFO_CPU
  ser.print("\"cpu\": \"",  BLYNK_INFO_CPU, "\",\n    ");
#endif
#ifdef BOARD_INFO_MHZ
  ser.print("\"mhz\": ",  BOARD_INFO_MHZ, ",\n    ");
#endif
  ser.print(JS[1]);

  for (int i = 0; i < BOARD_DIGITAL_MAX; i++) {
    ser.print("\"D", i, "\": ", i);
    if (i % 5 != 4) {
      ser.print(", ");
    } else {
      ser.print(",\n                 ");
    }
  }

  ser.print(JS[2]);

  for (int i = 0; i < BOARD_ANALOG_IN_MAX; i++) {
    int pin = analogInputToDigitalPin(i);
    if (pin != -1) {
      ser.print("\"A", i, "\": ", pin);
      if (i % 5 != 4) {
        ser.print(", ");
      } else {
        ser.print(",\n                 ");
      }
    }
  }

  ser.print(JS[3]);
  ser.print(BOARD_ANALOG_MAX);
  ser.print(JS[4]);

  for (int i = 0; i < BOARD_DIGITAL_MAX; i++) {
    bool hasPWM = digitalPinHasPWM(i);
    //bool hasInt = digitalPinToInterrupt(i) != NOT_AN_INTERRUPT;

    if (hasPWM) {
      ser.print("\"D", i, "\", ");
    }
  }

  ser.print(JS[5]);
  ser.print(BOARD_PWM_MAX);
  ser.print(JS[6]);
  ser.print(BOARD_VIRTUAL_MAX);
  ser.print(JS[7]);

  delay(10000);
}

