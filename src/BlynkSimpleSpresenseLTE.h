/**
 * @file       BlynkSimpleSpresenseLTE.h
 * @author     baggio
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2022 baggio
 * @date       May 2022
 * @brief
 *
 */

#ifndef BlynkSimpleSpresenseLTE_h
#define BlynkSimpleSpresenseLTE_h

#include <Adapters/BlynkSpresenseLTE.h>

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static BlynkArduinoClient _blynkTransport;
  BlynkSIM Blynk(_blynkTransport);
#else
  extern BlynkSIM Blynk;
#endif

#include <BlynkWidgets.h>

#endif
