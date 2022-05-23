/**
 * @file       BlynkSimpleWiFiNINA.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2018 Volodymyr Shymanskyy
 * @date       Sep 2018
 * @brief
 *
 */

#ifndef BlynkSimpleWiFiNINA_h
#define BlynkSimpleWiFiNINA_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "WiFiNINA"
#endif

#define BLYNK_SEND_ATOMIC
//#define BLYNK_USE_SSL

#include <WiFiNINA.h>
#include <Adapters/BlynkWiFiCommon.h>

//static WiFiSSLClient _blynkWifiClient;
static WiFiClient _blynkWifiClient;

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
  BlynkWifiCommon Blynk(_blynkTransport);
#else
  extern BlynkWifiCommon Blynk;
#endif

#include <BlynkWidgets.h>

#endif
