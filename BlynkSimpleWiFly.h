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

static BlynkTransportWiFly _blynkTransport;
BlynkWiFly Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
