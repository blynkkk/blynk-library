/**
 * @file       BlynkHandlers.h
 * @author     Volodymyr Shymanskyy
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
    int pin;
    int msg_id;
    int status;
};

typedef void (*WidgetReadHandler)(BlynkReq& request);
typedef void (*WidgetWriteHandler)(BlynkReq& request, const BlynkParam& param);

// Helper macro

#define BLYNK_ON_WRITE(pin) \
    void BlynkWidgetWrite ## pin (BlynkReq& request, const BlynkParam& param)

#define BLYNK_ON_READ(pin) \
    void BlynkWidgetRead ## pin (BlynkReq& request)

// Default read/write handlers (you can redefine them in your code)

void BlynkWidgetRead(BlynkReq& request);
void BlynkWidgetWrite(BlynkReq& request, const BlynkParam& param);

// Declare all pin handlers (you can redefine them in your code)

BLYNK_ON_READ(0 );
BLYNK_ON_READ(1 );
BLYNK_ON_READ(2 );
BLYNK_ON_READ(3 );
BLYNK_ON_READ(4 );
BLYNK_ON_READ(5 );
BLYNK_ON_READ(6 );
BLYNK_ON_READ(7 );
BLYNK_ON_READ(8 );
BLYNK_ON_READ(9 );
BLYNK_ON_READ(10);
BLYNK_ON_READ(11);
BLYNK_ON_READ(12);
BLYNK_ON_READ(13);
BLYNK_ON_READ(14);
BLYNK_ON_READ(15);
BLYNK_ON_READ(16);
BLYNK_ON_READ(17);
BLYNK_ON_READ(18);
BLYNK_ON_READ(19);
BLYNK_ON_READ(20);
BLYNK_ON_READ(21);
BLYNK_ON_READ(22);
BLYNK_ON_READ(23);
BLYNK_ON_READ(24);
BLYNK_ON_READ(25);
BLYNK_ON_READ(26);
BLYNK_ON_READ(27);
BLYNK_ON_READ(28);
BLYNK_ON_READ(29);
BLYNK_ON_READ(30);
BLYNK_ON_READ(31);

BLYNK_ON_WRITE(0 );
BLYNK_ON_WRITE(1 );
BLYNK_ON_WRITE(2 );
BLYNK_ON_WRITE(3 );
BLYNK_ON_WRITE(4 );
BLYNK_ON_WRITE(5 );
BLYNK_ON_WRITE(6 );
BLYNK_ON_WRITE(7 );
BLYNK_ON_WRITE(8 );
BLYNK_ON_WRITE(9 );
BLYNK_ON_WRITE(10);
BLYNK_ON_WRITE(11);
BLYNK_ON_WRITE(12);
BLYNK_ON_WRITE(13);
BLYNK_ON_WRITE(14);
BLYNK_ON_WRITE(15);
BLYNK_ON_WRITE(16);
BLYNK_ON_WRITE(17);
BLYNK_ON_WRITE(18);
BLYNK_ON_WRITE(19);
BLYNK_ON_WRITE(20);
BLYNK_ON_WRITE(21);
BLYNK_ON_WRITE(22);
BLYNK_ON_WRITE(23);
BLYNK_ON_WRITE(24);
BLYNK_ON_WRITE(25);
BLYNK_ON_WRITE(26);
BLYNK_ON_WRITE(27);
BLYNK_ON_WRITE(28);
BLYNK_ON_WRITE(29);
BLYNK_ON_WRITE(30);
BLYNK_ON_WRITE(31);

WidgetReadHandler GetReadHandler(unsigned pin);
WidgetWriteHandler GetWriteHandler(unsigned pin);

#endif
