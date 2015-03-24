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

struct BlynkReq
{
    uint16_t msg_id;
    uint16_t status;
    uint8_t pin;
};

typedef void (*WidgetReadHandler)(BlynkReq& request);
typedef void (*WidgetWriteHandler)(BlynkReq& request, const BlynkParam& param);

// Helper macro

#define BLYNK_WRITE(pin) \
    void BlynkWidgetWrite ## pin (BlynkReq& request, const BlynkParam& param)

#define BLYNK_READ(pin) \
    void BlynkWidgetRead ## pin (BlynkReq& request)

#define BLYNK_ATTACH_WIDGET(widget, pin)	\
    BLYNK_WRITE(pin) {					\
        (widget).onWrite(request, param);   \
    }

// Default read/write handlers (you can redefine them in your code)

void BlynkWidgetRead(BlynkReq& request);
void BlynkWidgetWrite(BlynkReq& request, const BlynkParam& param);

// Declare all pin handlers (you can redefine them in your code)

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

WidgetReadHandler GetReadHandler(unsigned pin);
WidgetWriteHandler GetWriteHandler(unsigned pin);

#endif
