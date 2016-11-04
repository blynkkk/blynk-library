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
    void onWrite(BlynkReq BLYNK_UNUSED &request, const BlynkParam BLYNK_UNUSED &param) {}

    void setAuthToken(const char* token) {
        char mem[BLYNK_MAX_SENDBYTES];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("i");
        cmd.add(token);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename T>
    void digitalWrite(const T& pin, int val) {
        char mem[BLYNK_MAX_SENDBYTES];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("dw");
        cmd.add(pin);
        cmd.add(val);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename T>
    void analogWrite(const T& pin, int val) {
        char mem[BLYNK_MAX_SENDBYTES];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("aw");
        cmd.add(pin);
        cmd.add(val);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename... Args>
    void virtualWrite(int pin, Args... values) {
        char mem[BLYNK_MAX_SENDBYTES];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("vw");
        cmd.add(pin);
        cmd.add_multi(values...);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    void virtualWriteBinary(int pin, const void* buff, size_t len) {
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("vw");
        cmd.add(pin);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength(), buff, len);
    }

    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWriteBinary(pin, param.getBuffer(), param.getLength()-1);
    }

private:
    int mPin;
};

#endif
