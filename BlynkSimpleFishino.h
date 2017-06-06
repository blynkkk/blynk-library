/**
 * @file       BlynkSimpleFishino.h
 * @author     Massimo Del Fedele
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Massimo Del Fedele
 * @date       April 2016
 * @brief
 *
 */
#ifndef BlynkSimpleFishino_h
#define BlynkSimpleFishino_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "Fishino"
#endif

#include <Fishino.h>
#include <Adapters/BlynkFishino.h>

static FishinoClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifiCommon Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
