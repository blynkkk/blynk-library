/**
 * @file       BlynkApiLinux.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkApiLinux_h
#define BlynkApiLinux_h

#include <Blynk/BlynkApi.h>

#ifndef BLYNK_INFO_DEVICE
    #define BLYNK_INFO_DEVICE  "Linux"
#endif

#ifdef BLYNK_NO_INFO

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::sendInfo() {}

#else

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::sendInfo()
{
    static const char profile[] BLYNK_PROGMEM = "blnkinf\0"
        BLYNK_PARAM_KV("ver"    , BLYNK_VERSION)
        BLYNK_PARAM_KV("h-beat" , BLYNK_TOSTRING(BLYNK_HEARTBEAT))
        BLYNK_PARAM_KV("buff-in", BLYNK_TOSTRING(BLYNK_MAX_READBYTES))
#ifdef BLYNK_INFO_DEVICE
        BLYNK_PARAM_KV("dev"    , BLYNK_INFO_DEVICE)
#endif
#ifdef BLYNK_INFO_CPU
        BLYNK_PARAM_KV("cpu"    , BLYNK_INFO_CPU)
#endif
#ifdef BLYNK_INFO_CONNECTION
        BLYNK_PARAM_KV("con"    , BLYNK_INFO_CONNECTION)
#endif
#ifdef BOARD_FIRMWARE_VERSION
        BLYNK_PARAM_KV("fw"     , BOARD_FIRMWARE_VERSION)
#endif
        BLYNK_PARAM_KV("build"  , __DATE__ " " __TIME__)
        "\0"
    ;
    const size_t profile_len = sizeof(profile)-8-2;

    char mem_dyn[64];
    BlynkParam profile_dyn(mem_dyn, 0, sizeof(mem_dyn));
    profile_dyn.add_key("conn", "Socket");
#ifdef BOARD_TEMPLATE_ID
    profile_dyn.add_key("tmpl", BOARD_TEMPLATE_ID);
#endif

    static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_INTERNAL, 0, profile+8, profile_len, profile_dyn.getBuffer(), profile_dyn.getLength());
    return;
}

#endif

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::processCmd(const void* buff, size_t len)
{
    BlynkParam param((void*)buff, len);
    BlynkParam::iterator it = param.begin();
    if (it >= param.end())
        return;
    const char* cmd = it.asStr();
    uint16_t cmd16;
    memcpy(&cmd16, cmd, sizeof(cmd16));
    if (++it >= param.end())
        return;

    const uint8_t pin = it.asInt();

    switch(cmd16) {

#ifndef BLYNK_NO_BUILTIN

    case BLYNK_HW_PM: {
        while (it < param.end()) {
            ++it;
#ifdef BLYNK_DEBUG
            BLYNK_LOG4(BLYNK_F("Invalid pin "), pin, BLYNK_F(" mode "), it.asStr());
#endif
            ++it;
        }
    } break;
    case BLYNK_HW_DR: {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("dw");
        rsp.add(pin);
        rsp.add(0); // TODO
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, rsp.getBuffer(), rsp.getLength()-1);
    } break;
    case BLYNK_HW_DW: {
        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        // TODO: digitalWrite(pin, it.asInt() ? HIGH : LOW);
    } break;
    case BLYNK_HW_AW: {
        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        // TODO: analogWrite(pin, it.asInt());
    } break;

#endif

    case BLYNK_HW_VR: {
        BlynkReq req = { pin };
        WidgetReadHandler handler = GetReadHandler(pin);
        if (handler && (handler != BlynkWidgetRead)) {
            handler(req);
        } else {
            BlynkWidgetReadDefault(req);
        }
    } break;
    case BLYNK_HW_VW: {
        ++it;
        char* start = (char*)it.asStr();
        BlynkParam param2(start, len - (start - (char*)buff));
        BlynkReq req = { pin };
        WidgetWriteHandler handler = GetWriteHandler(pin);
        if (handler && (handler != BlynkWidgetWrite)) {
            handler(req, param2);
        } else {
            BlynkWidgetWriteDefault(req, param2);
        }
    } break;
    default:
        BLYNK_LOG2(BLYNK_F("Invalid HW cmd: "), cmd);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_RESPONSE, static_cast<Proto*>(this)->msgIdOutOverride, NULL, BLYNK_ILLEGAL_COMMAND);
    }
}

#endif
