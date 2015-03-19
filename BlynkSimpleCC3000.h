/**
 * @file       BlynkSimpleCC3000.h
 * @author     Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkSimpleCC3000_h
#define BlynkSimpleCC3000_h

#include <BlynkCC3000.h>
#include <Adafruit_CC3000.h>

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed

static BlynkTransportCC3000 _blynkTransport(cc3000);
BlynkCC3000 Blynk(cc3000, _blynkTransport);

#include <BlynkWidgets.h>

#endif
