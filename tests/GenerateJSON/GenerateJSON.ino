#include "BlynkApiArduino.h"
#include "Blynk/BlynkDetectDevice.h"

#if !defined(BOARD_PWM_MAX)
  #if   defined(PWMRANGE)
    #define BOARD_PWM_MAX    PWMRANGE
  #elif defined(PWM_RESOLUTION)
    #define BOARD_PWM_MAX    (2^(PWM_RESOLUTION)-1)
  #else
    #error "Cannot detect BOARD_PWM_MAX"
  #endif
#endif

#if !defined(BOARD_ANALOG_MAX)
  #if   defined(PWMRANGE)
    #define BOARD_ANALOG_MAX    PWMRANGE
  #elif defined(PWM_RESOLUTION)
    #define BOARD_ANALOG_MAX    (2^(PWM_RESOLUTION)-1)
  #else
    #error "Cannot detect BOARD_ANALOG_MAX"
  #endif
#endif

#if defined(BLYNK_USE_128_VPINS)
  #define BOARD_VIRTUAL_MAX 127
#else
  #define BOARD_VIRTUAL_MAX 31
#endif

const char* JS[] = {
R"raw(
{
    "name": ")raw", R"raw(",
    "map": {
        "digital": {
            "pins": {
                 )raw", R"raw(
            },
            "ops": [ "dr", "dw" ]
        },
        "analog": {
            "pins": {
                 )raw", R"raw(
            },
            "ops": [ "dr", "dw", "ar" ],
            "arRange": [ 0, )raw", R"raw( ]
        },
        "pwm": {
            "pins": [
                )raw", R"raw(
            ],
            "ops": [ "aw" ],
            "awRange": [ 0, )raw", R"raw( ]
        },
        "virtual":  {
            "pinsRange": [ 0, )raw", R"raw( ],
            "ops": [ "vr", "vw" ]
        }
    }
}
)raw" };
            
void setup() {
  Serial.begin(9600);
  delay(10);
}

void loop() {
  Serial.print(JS[0]);
  Serial.print(BLYNK_INFO_DEVICE);
  Serial.print(JS[1]);

  for (int i = 0; i < NUM_DIGITAL_PINS; i++) {
    Serial.print("\"D");
    Serial.print(i);
    Serial.print("\": ");
    Serial.print(i);
    if (i % 5 != 4) {
      Serial.print(", ");
    } else {
      Serial.print(",\n                 ");
    }
  }

  Serial.print(JS[2]);

  for (int i = 0; i < NUM_ANALOG_INPUTS; i++) {
    int pin = analogInputToDigitalPin(i);
    Serial.print("\"A");
    Serial.print(i);
    Serial.print("\": ");
    Serial.print(pin);
    if (i % 5 != 4) {
      Serial.print(", ");
    } else {
      Serial.print(",\n                 ");
    }
  }

  Serial.print(JS[3]);
  Serial.print(BOARD_ANALOG_MAX);
  Serial.print(JS[4]);

  for (int i = 0; i < NUM_DIGITAL_PINS; i++) {
    bool hasPWM = digitalPinHasPWM(i);
    bool hasInt = digitalPinToInterrupt(i) != NOT_AN_INTERRUPT;

    if (hasPWM) {
      Serial.print("\"D");
      Serial.print(i);
      Serial.print("\", ");
    }
  }

  Serial.print(JS[5]);
  Serial.print(BOARD_PWM_MAX);
  Serial.print(JS[6]);
  Serial.print(BOARD_VIRTUAL_MAX);
  Serial.print(JS[7]);
  delay(10000);
}

