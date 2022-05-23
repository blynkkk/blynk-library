/**
 * @file       BlynkSimpleWiFly.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleWiFly_h
#define BlynkSimpleWiFly_h

#include <Adapters/BlynkWiFly.h>

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static BlynkTransportWiFly _blynkTransport;
  BlynkWiFly Blynk(_blynkTransport);
#else
  extern BlynkWiFly Blynk;
#endif

#include <BlynkWidgets.h>

#endif
