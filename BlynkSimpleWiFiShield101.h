/**
 * @file       BlynkSimpleWiFiShield101.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2016
 * @brief
 *
 */

#ifndef BlynkSimpleWiFiShield101_h
#define BlynkSimpleWiFiShield101_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "WiFi101"
#endif

// TODO: Remove Fix for WiFi shield (it has a crazy ping duration)
//#define BLYNK_TIMEOUT_MS 6000UL

#include <WiFi101.h>
#include <Adapters/BlynkWiFiCommon.h>

static WiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifiCommon Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
