/**
 * @file       BlynkSimpleMKRNB.h
 * @author     Riccardo Rizzo
 * @license    This project is released under the MIT License (MIT)
 * @date       Dec 2018
 * @brief
 *
 */

#ifndef BlynkSimpleMKRNB_h
#define BlynkSimpleMKRNB_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "MKRNB"
#endif

#include <MKRNB.h>
#include <Adapters/BlynkArduinoNB.h>

static BlynkArduinoClient _blynkTransport;
BlynkSIM Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
