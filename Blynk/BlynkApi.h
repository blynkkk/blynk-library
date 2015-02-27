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
        static_cast<Proto*>(this)->send(cmd.getBuffer(), cmd.getLength());
    }

    void virtualWrite(int pin, const void* buff, size_t len) {
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("vw");
        cmd.add(pin);
        static_cast<Proto*>(this)->send(cmd.getBuffer(), cmd.getLength(), buff, len);
    }

    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWrite(pin, param.getBuffer(), param.getLength());
    }

protected:
    void processCmd(const void* buff, size_t len);

};

/**
 * Implementation
 */
#include <Arduino.h>

template<class Proto>
void BlynkApi<Proto>::processCmd(const void* buff, size_t len)
{
    BlynkParam param((void*)buff, len);
    BlynkParam::iterator it = param.begin();
    if (it >= param.end())
        return;
    const char* cmd = it.asStr();

    if (!strcmp(cmd, "info")) {
        char mem[128];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add_key("ver"    , BLYNK_VERSION);
        rsp.add_key("h-beat" , BLYNK_HEARTBEAT);
        rsp.add_key("buff-in", BLYNK_MAX_READBYTES);
#if   defined(__AVR_ATmega168__)
        rsp.add_key("cpu"    , "ATmega168");
        rsp.add_key("device" , "Arduino");
#elif defined(__AVR_ATmega328P__)
        rsp.add_key("cpu"    , "ATmega328P");
        rsp.add_key("device" , "Arduino");
#elif defined(__AVR_ATmega1280__)
        rsp.add_key("cpu"    , "ATmega1280");
        rsp.add_key("device" , "Arduino Mega");
#elif defined(__AVR_ATmega2560__)
        rsp.add_key("cpu"    , "ATmega2560");
        rsp.add_key("device" , "Arduino Mega");
#elif defined(__AVR_ATmega32U4__)
        rsp.add_key("cpu"    , "ATmega32U4");
        rsp.add_key("device" , "Arduino Leonardo");
#elif defined(__SAM3X8E__)
        rsp.add_key("cpu"    , "AT91SAM3X8E");
        rsp.add_key("device" , "Arduino Due");
#endif
        //rsp.add_key("arduino_ver", ARDUINO);
        //rsp.add_key("cpu_speed"  , F_CPU/1000000);

        static_cast<Proto*>(this)->send(rsp.getBuffer(), rsp.getLength());
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
        static_cast<Proto*>(this)->send(rsp.getBuffer(), rsp.getLength());
    } else if (!strcmp(cmd, "ar")) {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("aw");
        rsp.add(pin);
        rsp.add(analogRead(pin));
        static_cast<Proto*>(this)->send(rsp.getBuffer(), rsp.getLength());
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
                BLYNK_LOG("pinMode %u -> %s", pin, it.asStr());
                if (!strcmp(it.asStr(), "in")) {
                    pinMode(pin, INPUT);
                } else if (!strcmp(it.asStr(), "out")) {
                    pinMode(pin, OUTPUT);
                } else if (!strcmp(it.asStr(), "pu")) {
                    pinMode(pin, INPUT_PULLUP);
                } else {
                    //! \todo
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
