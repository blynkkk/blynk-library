/**
 * @file       BlynkHandlers.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Handlers for virtual pin operations
 *
 */

#ifndef BlynkHandlers_h
#define BlynkHandlers_h

#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkParam.h>

// Helper macro

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

#define BLYNK_CONCAT(a, b) a ## b
#define BLYNK_CONCAT2(a, b) BLYNK_CONCAT(a, b)

// Initial syntax:
#define BLYNK_WRITE_2(pin) \
    void BlynkWidgetWrite ## pin (BlynkReq& request, const BlynkParam& param)

#define BLYNK_READ_2(pin)  \
    void BlynkWidgetRead ## pin  (BlynkReq& request)

#define BLYNK_WRITE_DEFAULT() BLYNK_WRITE_2(Default)
#define BLYNK_READ_DEFAULT()  BLYNK_READ_2(Default)

#define BLYNK_WRITE(pin)      BLYNK_WRITE_2(pin)
#define BLYNK_READ(pin)       BLYNK_READ_2(pin)

// New, more readable syntax:
#define BLYNK_IN_2(pin)  \
    void BlynkWidgetWrite ## pin (BlynkReq& request, const BlynkParam& getValue)

#define BLYNK_OUT_2(pin) \
    void BlynkWidgetRead ## pin  (BlynkReq& request)

#define BLYNK_IN_DEFAULT()   BLYNK_IN_2(Default)
#define BLYNK_OUT_DEFAULT()  BLYNK_OUT_2(Default)

#define BLYNK_IN(pin)        BLYNK_IN_2(pin)
#define BLYNK_OUT(pin)       BLYNK_OUT_2(pin)

// Additional handlers
#define BLYNK_CONNECTED()    void BlynkOnConnected()
#define BLYNK_DISCONNECTED() void BlynkOnDisconnected()

// Advanced functions

class BlynkAttachWidgetHelper {
public:
	template<class T>
	BlynkAttachWidgetHelper(T& widget, uint8_t vPin) {
		widget.setVPin(vPin);
	}
};

#define BLYNK_ATTACH_WIDGET(widget, pin)	\
	static BlynkAttachWidgetHelper BLYNK_CONCAT2(blnk_widget_helper_, __COUNTER__)((widget), (pin)); \
    BLYNK_WRITE(pin) { (widget).onWrite(request, param); }

#define BLYNK_VAR_INT(name, pin)	int name;  \
    BLYNK_WRITE(pin) { name = param.asInt(); } \
    BLYNK_READ(pin)  { Blynk.virtualWrite(pin, name); }

#define BLYNK_VAR_LONG(name, pin)	long name;  \
    BLYNK_WRITE(pin) { name = param.asLong(); } \
    BLYNK_READ(pin)  { Blynk.virtualWrite(pin, name); }

#ifndef BLYNK_NO_FLOAT
#define BLYNK_VAR_DOUBLE(name, pin)	double name;  \
    BLYNK_WRITE(pin) { name = param.asDouble(); } \
    BLYNK_READ(pin)  { Blynk.virtualWrite(pin, name); }
#endif

#ifdef ARDUINO
#define BLYNK_VAR_STRING(name, pin)	String name;  \
    BLYNK_WRITE(pin) { name = param.asStr(); } \
    BLYNK_READ(pin)  { Blynk.virtualWrite(pin, name); }
#endif

// Default read/write handlers (you can redefine them in your code)
#ifdef __cplusplus
extern "C" {
#endif

struct BlynkReq
{
    uint8_t pin;
};

typedef void (*WidgetReadHandler)(BlynkReq& request);
typedef void (*WidgetWriteHandler)(BlynkReq& request, const BlynkParam& param);

WidgetReadHandler GetReadHandler(uint8_t pin);
WidgetWriteHandler GetWriteHandler(uint8_t pin);

// Declare placeholders
BLYNK_READ();
BLYNK_WRITE();
void BlynkNoOpCbk();

// Declare all pin handlers (you can redefine them in your code)
BLYNK_CONNECTED();
BLYNK_DISCONNECTED();

BLYNK_READ_DEFAULT();
BLYNK_WRITE_DEFAULT();

BLYNK_READ(0 );
BLYNK_READ(1 );
BLYNK_READ(2 );
BLYNK_READ(3 );
BLYNK_READ(4 );
BLYNK_READ(5 );
BLYNK_READ(6 );
BLYNK_READ(7 );
BLYNK_READ(8 );
BLYNK_READ(9 );
BLYNK_READ(10);
BLYNK_READ(11);
BLYNK_READ(12);
BLYNK_READ(13);
BLYNK_READ(14);
BLYNK_READ(15);
BLYNK_READ(16);
BLYNK_READ(17);
BLYNK_READ(18);
BLYNK_READ(19);
BLYNK_READ(20);
BLYNK_READ(21);
BLYNK_READ(22);
BLYNK_READ(23);
BLYNK_READ(24);
BLYNK_READ(25);
BLYNK_READ(26);
BLYNK_READ(27);
BLYNK_READ(28);
BLYNK_READ(29);
BLYNK_READ(30);
BLYNK_READ(31);

BLYNK_WRITE(0 );
BLYNK_WRITE(1 );
BLYNK_WRITE(2 );
BLYNK_WRITE(3 );
BLYNK_WRITE(4 );
BLYNK_WRITE(5 );
BLYNK_WRITE(6 );
BLYNK_WRITE(7 );
BLYNK_WRITE(8 );
BLYNK_WRITE(9 );
BLYNK_WRITE(10);
BLYNK_WRITE(11);
BLYNK_WRITE(12);
BLYNK_WRITE(13);
BLYNK_WRITE(14);
BLYNK_WRITE(15);
BLYNK_WRITE(16);
BLYNK_WRITE(17);
BLYNK_WRITE(18);
BLYNK_WRITE(19);
BLYNK_WRITE(20);
BLYNK_WRITE(21);
BLYNK_WRITE(22);
BLYNK_WRITE(23);
BLYNK_WRITE(24);
BLYNK_WRITE(25);
BLYNK_WRITE(26);
BLYNK_WRITE(27);
BLYNK_WRITE(28);
BLYNK_WRITE(29);
BLYNK_WRITE(30);
BLYNK_WRITE(31);

#ifdef __cplusplus
}
#endif

#endif
