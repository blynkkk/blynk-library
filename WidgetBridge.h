/**
 * @file       WidgetBridge.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Apr 2015
 * @brief
 *
 */

#ifndef WidgetBridge_h
#define WidgetBridge_h

#include <Blynk/BlynkApi.h>

class WidgetBridge
{
public:
    WidgetBridge(int vPin)
        : mPin(vPin)
    {}
    void onWrite(BlynkReq& request, const BlynkParam& param) {}

    void setAuthToken(const char* token) {
        char mem[64];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("i");
        cmd.add(token);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename T>
    void digitalWrite(const T& pin, int val) {
        char mem[64];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("dw");
        cmd.add(pin);
        cmd.add(val);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename T>
    void analogWrite(const T& pin, int val) {
        char mem[64];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("aw");
        cmd.add(pin);
        cmd.add(val);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename T>
    void virtualWrite(int pin, const T& data) {
        char mem[64];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("vw");
        cmd.add(pin);
        cmd.add(data);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    void virtualWrite(int pin, const void* buff, size_t len) {
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("vw");
        cmd.add(pin);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength(), buff, len);
    }

    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWrite(pin, param.getBuffer(), param.getLength()-1);
    }

private:
    int mPin;
};

#endif
