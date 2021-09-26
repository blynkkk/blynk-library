/**
 * @file       BlynkSimpleCC3000.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 */

#ifndef BlynkSimpleCC3000_h
#define BlynkSimpleCC3000_h

#include <Adapters/BlynkCC3000.h>
#include <Adafruit_CC3000.h>

// Use hardware SPI for the remaining pins
// SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static BlynkTransportCC3000 _blynkTransport(cc3000);
  BlynkCC3000 Blynk(cc3000, _blynkTransport);
#else
  extern BlynkCC3000 Blynk;
#endif

#include <BlynkWidgets.h>

#endif
