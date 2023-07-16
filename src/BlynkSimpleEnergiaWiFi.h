/**
 * @file       BlynkSimpleEnergiaWiFi.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleEnergiaWiFi_h
#define BlynkSimpleEnergiaWiFi_h

#include <WiFi.h>
#include <Adapters/BlynkWiFiCommon.h>

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static WiFiClient _blynkWifiClient;
  static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
  BlynkWifiCommon Blynk(_blynkTransport);
#else
  extern BlynkWifiCommon Blynk;
#endif

#include <BlynkWidgets.h>

#endif
