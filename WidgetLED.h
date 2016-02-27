/**
 * @file       WidgetLED.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Apr 2015
 * @brief
 */

#ifndef WidgetLED_h
#define WidgetLED_h

#include <Blynk/BlynkApi.h>

class WidgetLED
{
public:
	WidgetLED(uint8_t pin) : mPin(pin) {}
    void setVPin(int vPin) { mPin = vPin; }

#ifdef BLYNK_EXPERIMENTAL

    void setRGB(uint8_t r, uint8_t g, uint8_t b) {
        char mem[64] = "";
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("rgb");
        cmd.add(r);
        cmd.add(g);
        cmd.add(b);
        Blynk.virtualWrite(mPin, cmd);
    }

    void setHSV(uint8_t h, uint8_t s, uint8_t v) {
        char mem[64] = "";
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("hsv");
        cmd.add(h);
        cmd.add(s);
        cmd.add(v);
        Blynk.virtualWrite(mPin, cmd);
    }

#endif

    uint8_t getValue() const {
        return mValue;
    }

    void setValue(uint8_t value) {
    	mValue = value;
        Blynk.virtualWrite(mPin, value);
    }

    void on() {
    	setValue(255);
    }

    void off() {
    	setValue(0);
    }

private:
    uint8_t mPin;
    uint8_t mValue;
};

#endif
