/**
 * @file       BlynkSimpleTinyGSM.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Nov 2016
 * @brief
 *
 */

#ifndef BlynkSimpleTinyGSM_h
#define BlynkSimpleTinyGSM_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "TinyGSM"
#endif

#include <Adapters/BlynkGsmClient.h>

static BlynkArduinoClient _blynkTransport;
BlynkSIM Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
