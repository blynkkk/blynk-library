#ifndef BlynkApi_h
#define BlynkApi_h

#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkParam.h>
#include <Blynk/Delegate.h>
#include <Arduino.h>

struct BlynkReq
{
    int pin;
    int msg_id;
    int status;
};

#if 1
    typedef Delegate< void(void) > EventHandler;
    typedef Delegate< void(BlynkReq&) > WidgetReadHandler;
    typedef Delegate< void(BlynkReq&, const BlynkParam&) > WidgetWriteHandler;
#else
    typedef void (*EventHandler) (void);
    typedef void (*WidgetReadHandler)(BlynkReq& req);
    typedef void (*WidgetWriteHandler)(BlynkReq& req, const BlynkParam& param);
#endif

template <class Proto>
class BlynkApi
{
public:

public:
    BlynkApi()
    {
        memset(vPinsR, 0, sizeof(vPinsR));
        memset(vPinsW, 0, sizeof(vPinsW));
    }

    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWrite(pin, param.getBuffer(), param.getLength());
    }

    void virtualWrite(int pin, int val) {
        virtualWrite(pin, "1024", 4); //! \todo
    }

    void virtualWrite(int pin, unsigned long val) {
        virtualWrite(pin, "1024", 4); //! \todo
    }

    void virtualWrite(int pin, const void* buff, size_t len) {
        static_cast<Proto*>(this)->send(BLYNK_CMD_HARDWARE, buff, len); //! \todo
    }

    //! \todo virtualRead

    void attachVitrualPin(int pin, WidgetReadHandler h)  { vPinsR[pin] = h; }
    void attachVitrualPin(int pin, WidgetWriteHandler h) { vPinsW[pin] = h; }
    void onConnect(EventHandler h) {} //! \todo
    void onDisconnect(EventHandler h) {} //! \todo

protected:
    void processCmd(BlynkParam& param);

private:
    WidgetReadHandler  vPinsR[BLYNK_MAX_HANDLERS];
    WidgetWriteHandler vPinsW[BLYNK_MAX_HANDLERS];
};

template<class Proto>
void BlynkApi<Proto>::processCmd(BlynkParam& param)
{
    BlynkParam::iterator it = param.begin();
    if (it == param.end())
        return;
    const char* cmd = it.asStr();
    if (++it == param.end())
        return;
    const int pin = it.asInt();

    if (       !strcmp(cmd, "dr")) {
        virtualWrite(pin, digitalRead(pin));
    } else if (!strcmp(cmd, "ar")) {
        virtualWrite(pin, analogRead(pin));
    } else if (!strcmp(cmd, "vr") && vPinsR[pin]) {
        BlynkReq req;
        vPinsR[pin](req);
    } else {

        if (!strcmp(cmd, "vw") && vPinsW[pin]) {
            BlynkReq req;
            vPinsW[pin](req, param); //! \todo
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


struct BlynkAttachPinHelper
{
    template <class T>
    BlynkAttachPinHelper(T& blynk, int pin, WidgetReadHandler r) {
        blynk.attachVitrualPin(pin, r);
    }
    template <class T>
    BlynkAttachPinHelper(T& blynk, int pin, WidgetWriteHandler w) {
        blynk.attachVitrualPin(pin, w);
    }
};

#define BLYNK_ON_WRITE_EX(blynk, pin) \
    static void __OnWidgetWrite ## pin (BlynkReq& req, const BlynkParam& param); \
    static BlynkAttachPinHelper __OnWidgetWriteHelper ## pin(Blynk, pin, __OnWidgetWrite ## pin); \
    void __OnWidgetWrite ## pin (BlynkReq& req, const BlynkParam& param)

#define BLYNK_ON_READ_EX(blynk, pin) \
    static void __OnWidgetRead ## pin (BlynkReq& req); \
    static BlynkAttachPinHelper __OnWidgetReadHelper ## pin(Blynk, pin, __OnWidgetRead ## pin); \
    void __OnWidgetRead ## pin (BlynkReq& req)

// Use the _EX variant if you want to specify the Blynk instance
#define BLYNK_ON_WRITE(pin) BLYNK_ON_WRITE_EX(Blynk, pin)
#define BLYNK_ON_READ(pin) BLYNK_ON_READ_EX(Blynk, pin)

// Define virtual pins just for visual context
#define V0  0
#define V1  1
#define V2  2
#define V3  3
#define V4  4
#define V5  5
#define V6  6
#define V7  7
#define V8  8
#define V9  9
#define V10 10
#define V11 11
#define V12 12
#define V13 13
#define V14 14
#define V15 15
#define V16 16
#define V17 17
#define V18 18
#define V19 19
#define V20 20
#define V21 21
#define V22 22
#define V23 23
#define V24 24
#define V25 25
#define V26 26
#define V27 27
#define V28 28
#define V29 29
#define V30 30
#define V31 31
#define V32 32
#define V33 33
#define V34 34
#define V35 35
#define V36 36
#define V37 37
#define V38 38
#define V39 39
#define V40 40
#define V41 41
#define V42 42
#define V43 43
#define V44 44
#define V45 45
#define V46 46
#define V47 47
#define V48 48
#define V49 49
#define V50 50
#define V51 51
#define V52 52
#define V53 53
#define V54 54
#define V55 55
#define V56 56
#define V57 57
#define V58 58
#define V59 59
#define V60 60
#define V61 61
#define V62 62
#define V63 63

#endif
