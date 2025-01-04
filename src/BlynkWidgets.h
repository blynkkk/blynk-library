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
#include <WidgetTimeInput.h>
#include <WidgetMap.h>

// Cannot auto-include as these have library dependencies
//#include <WidgetRTC.h>

template<class T>
class VPinWriteOnChange {

public:
    VPinWriteOnChange(int vpin)
        : _vPin(vpin)
    {
        reset();
    }

    VPinWriteOnChange<T>& operator= (const T& value) {
        update(value);
        return *this;
    }

    void reset() {
        _hasValue = false;
    }

    bool hasValue() const {
        return _hasValue;
    }

    void set(const T& value) {
        _value = value;
        _hasValue = true;
    }

    void update(const T& value) {
        if (!hasValue() || value != _value) {
            set(value);
            report();
        }
    }

    void report() {
        if (hasValue()) {
            Blynk.virtualWrite(_vPin, _value);
        }
    }

private:
    const int   _vPin;
    T           _value;
    bool        _hasValue;
};

template<>
class VPinWriteOnChange<float> {

public:
    VPinWriteOnChange(int vpin, float threshold)
        : _vPin(vpin)
        , _threshold(threshold)
    {
        reset();
    }

    VPinWriteOnChange<float>& operator= (const float& value) {
        update(value);
        return *this;
    }

    void reset() {
        _value = NAN;
    }

    bool hasValue() const {
        return !isnan(_value);
    }

    void set(const float& value) {
        _value = value;
    }

    void update(const float& value) {
        if (!hasValue() || fabs(value - _value) > _threshold) {
            set(value);
            report();
        }
    }

    void report() {
        if (hasValue()) {
            Blynk.virtualWrite(_vPin, _value);
        }
    }

private:
    const int   _vPin;
    float       _value;
    float       _threshold;
};
