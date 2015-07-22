///
/// @file		BlynkSimpleCC3200.h
/// @brief		Header
/// @details	LaunchPad CC3200 Adapter for Blink
/// @n
/// @n @b		Project Blink_CC3200
/// @n          Extension for Blink by Volodymyr Shymanskyy
/// @see        http://www.blynk.cc
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
///
/// @date		July 22, 2015 19:00
/// @version	101
///
/// @copyright	(c) Rei Vilo, 2015
/// @copyright	CC = BY CA NC
///
/// @see		ReadMe.txt for references
///

/**
 * @file       BlynkSimpleCC3200.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 * Rei Vilo, Jul 21, 2015
 */

#ifndef BlynkSimpleCC3200_h
#define BlynkSimpleCC3200_h

#include <Adapters/BlynkCC3200.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

// Use hardware SPI for the remaining pins
// SCK = 13, MISO = 12, and MOSI = 11

//Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
//                                         SPI_CLOCK_DIVIDER); // you can change this clock speed
//
//static BlynkTransportCC3000 _blynkTransport(cc3000);
//BlynkCC3000 Blynk(cc3000, _blynkTransport);
//
//#include <BlynkWidgets.h>

WiFiClass cc3000;

static BlynkTransportCC3200 _blynkTransport(cc3000);
BlynkCC3200 Blynk(cc3000, _blynkTransport);

#include <BlynkWidgets.h>

#endif
