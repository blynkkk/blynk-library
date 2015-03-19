/**
 * @file       BlynkSimpleWiFly.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleWiFly_h
#define BlynkSimpleWiFly_h

#include <BlynkWiFly.h>

static BlynkTransportWiFly _blynkTransport;
BlynkWiFly Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
