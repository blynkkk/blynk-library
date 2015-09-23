/**
 * @file       BlynkSimpleSerial.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleSerial_h
#define BlynkSimpleSerial_h

#include <Adapters/BlynkSerial.h>

#if defined(__SAMD21G18A__)
    // For Arduino Zero (M0)
    typedef BlynkTransportSerialChecked<Uart> ArduinoHwSerial;
#elif defined(__SAM3X8E__)
    // For Arduino Due
    typedef BlynkTransportSerialChecked<UARTClass> ArduinoHwSerial;
#elif defined(USBCON)
    // For versions with hw USB, like Micro
    typedef BlynkTransportSerialChecked<Serial_> ArduinoHwSerial;
#else
    // For versions with UART
    typedef BlynkTransportSerial<HardwareSerial> ArduinoHwSerial;
#endif

static ArduinoHwSerial _blynkTransport(Serial);
BlynkSerial<ArduinoHwSerial> Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
