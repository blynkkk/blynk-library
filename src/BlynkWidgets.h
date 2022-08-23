/**
 * @file       BlynkWidgets.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 */

#include <WidgetLED.h>
#include <WidgetLCD.h>
#include <WidgetTerminal.h>
#include <WidgetBridge.h>
#include <WidgetTimeInput.h>
#include <WidgetTable.h>
#include <WidgetGPS.h>
#include <WidgetMap.h>

// Cannot auto-include as these have library dependencies
//#include <WidgetRTC.h>
//#include <WidgetSD.h>

template<class T>
class VPinWriteOnChange {
    T prev;
    const int vpin;
public:
    VPinWriteOnChange(int v)
        : vpin(v)
    {}

    VPinWriteOnChange<T>& operator= (const T& value) {
        update(value);
        return *this;
    }

    void set(const T& value) {
        prev = value;
    }

    void update(const T& value) {
        if (value != prev) {
            prev = value;
            report();
        }
    }

    void report() {
        Blynk.virtualWrite(vpin, prev);
    }
};
