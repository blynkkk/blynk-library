/**
 * @file BlynkSimpleWizFi310.h
 * @
 * @license This project is released under the MIT License (MIT)
 * @copyright Copyright (c) 2015 Volodymyr Shymanskyy
 * @date Oct 2017
 * @brief
 *
 */
#ifndef BlynkSimpleWizFi250_h
#define BlynkSimpleWizFi250_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "WizFi250"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 64

#include <WizFi250.h>
#include <Adapters/BlynkWizFi250.h>

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static WiFiClient _blynkWifiClient;
  static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
  BlynkWifiCommon Blynk(_blynkTransport);
#else
  extern BlynkWifiCommon Blynk;
#endif

#include <BlynkWidgets.h>

#endif
