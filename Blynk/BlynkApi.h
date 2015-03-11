/**
 * @file       BlynkApi.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      High-level functions
 *
 */

#ifndef BlynkApi_h
#define BlynkApi_h

#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkDebug.h>
#include <Blynk/BlynkParam.h>
#include <Blynk/BlynkHandlers.h>

/**
* A test class. A more elaborate class description.
*/
template <class Proto>
class BlynkApi
{
public:
    template <typename T>
    void virtualWrite(int pin, const T& data) {
        char mem[64];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("vw");
        cmd.add(pin);
        cmd.add(data);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength());
    }

    void virtualWrite(int pin, const void* buff, size_t len) {
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("vw");
        cmd.add(pin);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength(), buff, len);
    }

    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWrite(pin, param.getBuffer(), param.getLength());
    }

    void tweet(const char* msg) {
        size_t len = strlen(msg);
        if (len < 140) {
            static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_TWEET, 0, msg, len);
        }
    }

#if defined(BLYNK_HAS_DELAY)

    void delay(unsigned long ms) {
        uint16_t start = (uint16_t)micros();
        while (ms > 0) {
            static_cast<Proto*>(this)->run();
            if (((uint16_t)micros() - start) >= 1000) {
                ms--;
                start += 1000;
            }
        }
    }

#endif

protected:
    void processCmd(const void* buff, size_t len);

};

/**
 * Implementation
 */
#include <Arduino.h>

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::processCmd(const void* buff, size_t len)
{
    BlynkParam param((void*)buff, len);
    BlynkParam::iterator it = param.begin();
    if (it >= param.end())
        return;
    const char* cmd = it.asStr();

    if (!strcmp(cmd, "info")) {
        static const char profile[] BLYNK_PROGMEM =
            BLYNK_PARAM_KV("ver"    , BLYNK_VERSION)
            BLYNK_PARAM_KV("h-beat" , TOSTRING(BLYNK_HEARTBEAT))
            BLYNK_PARAM_KV("buff-in", TOSTRING(BLYNK_MAX_READBYTES))
#if   defined(__AVR_ATmega168__)
            BLYNK_PARAM_KV("cpu"    , "ATmega168")
            BLYNK_PARAM_KV("device" , "Arduino")
#elif defined(__AVR_ATmega328P__)
            BLYNK_PARAM_KV("cpu"    , "ATmega328P")
            BLYNK_PARAM_KV("device" , "Arduino")
#elif defined(__AVR_ATmega1280__)
            BLYNK_PARAM_KV("cpu"    , "ATmega1280")
            BLYNK_PARAM_KV("device" , "Arduino Mega")
#elif defined(__AVR_ATmega2560__)
            BLYNK_PARAM_KV("cpu"    , "ATmega2560")
            BLYNK_PARAM_KV("device" , "Arduino Mega")
#elif defined(__AVR_ATmega32U4__)
            BLYNK_PARAM_KV("cpu"    , "ATmega32U4")
            BLYNK_PARAM_KV("device" , "Arduino Leonardo")
#elif defined(__SAM3X8E__)
            BLYNK_PARAM_KV("cpu"    , "AT91SAM3X8E")
            BLYNK_PARAM_KV("device" , "Arduino Due")
#endif
        ;
        const size_t profile_len = sizeof(profile)-1;

        char mem_dyn[32];
        BlynkParam profile_dyn(mem_dyn, 0, sizeof(mem_dyn));
        profile_dyn.add_key("conn", "Ethernet");
        //profile_dyn.add_key("conn", "Serial");

#ifdef BLYNK_HAS_PROGMEM
        char mem[profile_len];
        memcpy_P(mem, profile, profile_len);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, mem, profile_len, profile_dyn.getBuffer(), profile_dyn.getLength());
#else
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, profile, profile_len, profile_dyn.getBuffer(), profile_dyn.getLength());
#endif
        return;
    }

    if (++it >= param.end())
        return;
    const unsigned pin = it.asInt();

    if (!strcmp(cmd, "dr")) {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("dw");
        rsp.add(pin);
        rsp.add(digitalRead(pin));
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, rsp.getBuffer(), rsp.getLength());
    } else if (!strcmp(cmd, "ar")) {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("aw");
        rsp.add(pin);
        rsp.add(analogRead(pin));
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, rsp.getBuffer(), rsp.getLength());
    } else if (!strcmp(cmd, "vr")) {
        if (WidgetReadHandler handler = GetReadHandler(pin)) {
            BlynkReq req = { pin, 0, BLYNK_SUCCESS };
            handler(req);
        }
    } else {

        if (!strcmp(cmd, "vw")) {
            ++it;
            if (WidgetWriteHandler handler = GetWriteHandler(pin)) {
                BlynkReq req = { pin, 0, BLYNK_SUCCESS };
                char* start = (char*)it.asStr();
                BlynkParam param2(start, len - (start - (char*)buff));
                handler(req, param2);
            }
            return;
        }

        if (!strcmp(cmd, "pm")) {
            while (it < param.end()) {
                unsigned pin = it.asInt();
                ++it;
                //BLYNK_LOG("pinMode %u -> %s", pin, it.asStr());
                if (!strcmp(it.asStr(), "in")) {
                    pinMode(pin, INPUT);
                } else if (!strcmp(it.asStr(), "out")) {
                    pinMode(pin, OUTPUT);
                } else if (!strcmp(it.asStr(), "pu")) {
                    pinMode(pin, INPUT_PULLUP);
                } else {
#ifdef BLYNK_DEBUG
                    BLYNK_LOG("Invalid pinMode %u -> %s", pin, it.asStr());
#endif
                }
                ++it;
            }
        }

        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        if (!strcmp(cmd, "dw")) {
            //BLYNK_LOG("digitalWrite %d -> %d", pin, it.asInt());
            digitalWrite(pin, it.asInt() ? HIGH : LOW);
        } else if (!strcmp(cmd, "aw")) {
            //BLYNK_LOG("analogWrite %d -> %d", pin, it.asInt());
            analogWrite(pin, it.asInt());
        } else {
            BLYNK_LOG("Invalid HW cmd: %s", cmd);
        }
    }
}

#endif
