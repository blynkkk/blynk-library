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
    void virtualWrite(int pin, int value) {
        char str[2 + 8 * sizeof(value)];
        itoa(value, str, 10);
        virtualWrite(pin, str);
    }

    void virtualWrite(unsigned int pin, int value) {
        char str[1 + 8 * sizeof(value)];
        utoa(value, str, 10);
        virtualWrite(pin, str);
    }

    void virtualWrite(int pin, long value) {
        char str[2 + 8 * sizeof(value)];
        ltoa(value, str, 10);
        virtualWrite(pin, str);
    }

    void virtualWrite(int pin, unsigned long value) {
        char str[1 + 8 * sizeof(value)];
        ultoa(value, str, 10);
        virtualWrite(pin, str);
    }

    void virtualWrite(int pin, float value) {
        char str[33];
        dtostrf(value, 5, 3, str);
        virtualWrite(pin, str);
    }

    void virtualWrite(int pin, double value) {
        char str[33];
        dtostrf(value, 5, 3, str);
        virtualWrite(pin, str);
    }

    void virtualWrite(int pin, const char* str) {
        virtualWrite(pin, str, strlen(str));
    }

    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWrite(pin, param.getBuffer(), param.getLength());
    }

    void virtualWrite(int pin, const void* buff, size_t len) {
        static_cast<Proto*>(this)->send(buff, len);
    }

    //! \todo virtualRead

    //void attachVitrualPin(int pin, WidgetReadHandler h)  { vPinsR[pin] = h; }
    //void attachVitrualPin(int pin, WidgetWriteHandler h) { vPinsW[pin] = h; }
    //void onConnect(EventHandler h) {} //! \todo
    //void onDisconnect(EventHandler h) {} //! \todo

protected:
    void processCmd(BlynkParam& param);

};

/**
 * Implementation
 */
#include <Arduino.h>

template<class Proto>
void BlynkApi<Proto>::processCmd(BlynkParam& param)
{
    BlynkParam::iterator it = param.begin();
    if (it == param.end())
        return;
    const char* cmd = it.asStr();
    if (++it == param.end())
        return;
    const unsigned pin = it.asInt();

    if (       !strcmp(cmd, "dr")) {
        virtualWrite(pin, digitalRead(pin));
    } else if (!strcmp(cmd, "ar")) {
        virtualWrite(pin, analogRead(pin)); //! \todo A0+pin?
    } else if (!strcmp(cmd, "vr")) {
        if (WidgetReadHandler handler = GetReadHandler(pin)) {
            BlynkReq req = { pin, 0, BLYNK_SUCCESS };
            handler(req);
        }
    } else {
        if (!strcmp(cmd, "vw")) {
            if (WidgetWriteHandler handler = GetWriteHandler(pin)) {
                BlynkReq req = { pin, 0, BLYNK_SUCCESS };
                handler(req, param); //! \todo
            }
            return;
        }

        if (++it == param.end())
            return;

        if (!strcmp(cmd, "pm")) {
            BLYNK_LOG("pinMode %d -> %s", pin, it.asStr());
            if (!strcmp(it.asStr(), "in")) {
                pinMode(pin, INPUT);
            } else if (!strcmp(it.asStr(), "out")) {
                pinMode(pin, OUTPUT);
            } else if (!strcmp(it.asStr(), "pu")) {
                pinMode(pin, INPUT_PULLUP);
            } else {
                //! \todo
            }
        } else if (!strcmp(cmd, "dw")) {
            BLYNK_LOG("digitalWrite %d -> %d", pin, it.asInt());
            digitalWrite(pin, it.asInt() ? HIGH : LOW);
        } else if (!strcmp(cmd, "aw")) {
            BLYNK_LOG("analogWrite %d -> %d", pin, it.asInt());
            analogWrite(pin, it.asInt());
        } else {
            BLYNK_LOG("Invalid HW cmd");
        }
    }
}

#endif
