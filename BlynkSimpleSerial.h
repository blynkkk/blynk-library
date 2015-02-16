/**
 * @file       BlynkSimpleSerial.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      
 *
 */

#ifndef BlynkSimpleSerial_h
#define BlynkSimpleSerial_h

#include <BlynkSerial.h>

#if defined(USBCON)
    // For versions with hw USB, like Micro
    typedef BlynkTransportSerialChecked<Serial_> ArduinoHwSerial;
#else
    // For versions with UART
    typedef BlynkTransportSerial<HardwareSerial> ArduinoHwSerial;
#endif

static ArduinoHwSerial _blynkTransport(Serial);
BlynkSerial<ArduinoHwSerial> Blynk(_blynkTransport);

#endif
