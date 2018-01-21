/**
 * @file BlynkSimpleWifiLink.h
 * @
 * @license This project is released under the MIT License (MIT)
 * @copyright Copyright (c) 2015 Volodymyr Shymanskyy
 * @date Jan 2017
 * @brief
 *
 */
#ifndef BlynkSimpleWiFiLink_h
#define BlynkSimpleWiFiLink_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "WiFiLink"
#endif

#define BLYNK_SEND_ATOMIC
#define BLYNK_SEND_CHUNK 64

#include <WiFiLink.h>
#include <Adapters/BlynkWiFiCommon.h>

static WiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifiCommon Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
