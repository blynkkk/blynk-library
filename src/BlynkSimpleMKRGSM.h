/**
 * @file       BlynkSimpleMKRGSM.h
 * @author     Riccardo Rizzo
 * @license    This project is released under the MIT License (MIT)
 * @date       Dec 2018
 * @brief
 *
 */

#ifndef BlynkSimpleMKRGSM_h
#define BlynkSimpleMKRGSM_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "MKRGSM"
#endif

#include <MKRGSM.h>
#include <Adapters/BlynkArduinoGSM.h>

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static BlynkArduinoClient _blynkTransport;
  BlynkSIM Blynk(_blynkTransport);
#else
  extern BlynkSIM Blynk;
#endif

#include <BlynkWidgets.h>

#endif
