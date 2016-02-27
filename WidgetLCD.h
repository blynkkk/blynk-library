/**
 * @file       WidgetLCD.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 */

#ifndef WidgetLCD_h
#define WidgetLCD_h

#include <Blynk/BlynkApi.h>

class WidgetLCD
{
public:
    WidgetLCD(uint8_t pin) : mPin(pin) {}
    void setVPin(int vPin) { mPin = vPin; }

    void clear() {
        Blynk.virtualWrite(mPin, "clr");
    }

    template<typename T>
    void print(int x, int y, const T& str) {
        char mem[64] = "";
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("p");
        cmd.add(x);
        cmd.add(y);
        cmd.add(str);
        Blynk.virtualWrite(mPin, cmd);
    }

private:
    uint8_t mPin;
};

#endif
