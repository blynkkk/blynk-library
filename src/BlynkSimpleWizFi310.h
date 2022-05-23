/**
 * @file BlynkSimpleWizFi310.h
 * @
 * @license This project is released under the MIT License (MIT)
 * @copyright Copyright (c) 2015 Volodymyr Shymanskyy
 * @date Oct 2017
 * @brief
 *
 */
#ifndef BlynkSimpleWizFi310_h
#define BlynkSimpleWizFi310_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "WizFi310"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 64

#include <WizFi310.h>
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
