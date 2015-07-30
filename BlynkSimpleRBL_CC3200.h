/**
 * @file       BlynkSimpleRBL_CC3200.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleRBL_CC3200_h
#define BlynkSimpleRBL_CC3200_h

#ifndef BLYNK_INFO_DEVICE
#define BLYNK_INFO_DEVICE  "RBL_CC3200"
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "CC3200"
#endif

#include <WiFi.h>
#include <Adapters/BlynkWiFiCommon.h>

static WiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifiCommon Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
