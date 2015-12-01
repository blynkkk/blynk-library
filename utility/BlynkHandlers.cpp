/**
 * @file       BlynkHandlers.cpp
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Virtual pin utilities
 */

#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkHandlers.h>
#include <Blynk/BlynkDebug.h>

void BlynkNoOpCbk()
{}

void BlynkWidgetRead(BlynkReq& request)
{
    BLYNK_LOG("No handler for reading from pin %d", request.pin);
}

void BlynkWidgetWrite(BlynkReq& request, const BlynkParam& param)
{
    BLYNK_LOG("No handler for writing to pin %d", request.pin);
}

#define BLYNK_ON_READ_IMPL(pin)  void BlynkWidgetRead  ## pin (BlynkReq& req) \
          __attribute__((weak, alias("BlynkWidgetRead")))

#define BLYNK_ON_WRITE_IMPL(pin) void BlynkWidgetWrite ## pin (BlynkReq& req, const BlynkParam& param) \
          __attribute__((weak, alias("BlynkWidgetWrite")))

BLYNK_CONNECTED() __attribute__((weak, alias("BlynkNoOpCbk")));
BLYNK_DISCONNECTED() __attribute__((weak, alias("BlynkNoOpCbk")));

BLYNK_ON_READ_IMPL(Default);
BLYNK_ON_WRITE_IMPL(Default);

BLYNK_ON_READ_IMPL(0 );
BLYNK_ON_READ_IMPL(1 );
BLYNK_ON_READ_IMPL(2 );
BLYNK_ON_READ_IMPL(3 );
BLYNK_ON_READ_IMPL(4 );
BLYNK_ON_READ_IMPL(5 );
BLYNK_ON_READ_IMPL(6 );
BLYNK_ON_READ_IMPL(7 );
BLYNK_ON_READ_IMPL(8 );
BLYNK_ON_READ_IMPL(9 );
BLYNK_ON_READ_IMPL(10);
BLYNK_ON_READ_IMPL(11);
BLYNK_ON_READ_IMPL(12);
BLYNK_ON_READ_IMPL(13);
BLYNK_ON_READ_IMPL(14);
BLYNK_ON_READ_IMPL(15);
BLYNK_ON_READ_IMPL(16);
BLYNK_ON_READ_IMPL(17);
BLYNK_ON_READ_IMPL(18);
BLYNK_ON_READ_IMPL(19);
BLYNK_ON_READ_IMPL(20);
BLYNK_ON_READ_IMPL(21);
BLYNK_ON_READ_IMPL(22);
BLYNK_ON_READ_IMPL(23);
BLYNK_ON_READ_IMPL(24);
BLYNK_ON_READ_IMPL(25);
BLYNK_ON_READ_IMPL(26);
BLYNK_ON_READ_IMPL(27);
BLYNK_ON_READ_IMPL(28);
BLYNK_ON_READ_IMPL(29);
BLYNK_ON_READ_IMPL(30);
BLYNK_ON_READ_IMPL(31);

BLYNK_ON_WRITE_IMPL(0 );
BLYNK_ON_WRITE_IMPL(1 );
BLYNK_ON_WRITE_IMPL(2 );
BLYNK_ON_WRITE_IMPL(3 );
BLYNK_ON_WRITE_IMPL(4 );
BLYNK_ON_WRITE_IMPL(5 );
BLYNK_ON_WRITE_IMPL(6 );
BLYNK_ON_WRITE_IMPL(7 );
BLYNK_ON_WRITE_IMPL(8 );
BLYNK_ON_WRITE_IMPL(9 );
BLYNK_ON_WRITE_IMPL(10);
BLYNK_ON_WRITE_IMPL(11);
BLYNK_ON_WRITE_IMPL(12);
BLYNK_ON_WRITE_IMPL(13);
BLYNK_ON_WRITE_IMPL(14);
BLYNK_ON_WRITE_IMPL(15);
BLYNK_ON_WRITE_IMPL(16);
BLYNK_ON_WRITE_IMPL(17);
BLYNK_ON_WRITE_IMPL(18);
BLYNK_ON_WRITE_IMPL(19);
BLYNK_ON_WRITE_IMPL(20);
BLYNK_ON_WRITE_IMPL(21);
BLYNK_ON_WRITE_IMPL(22);
BLYNK_ON_WRITE_IMPL(23);
BLYNK_ON_WRITE_IMPL(24);
BLYNK_ON_WRITE_IMPL(25);
BLYNK_ON_WRITE_IMPL(26);
BLYNK_ON_WRITE_IMPL(27);
BLYNK_ON_WRITE_IMPL(28);
BLYNK_ON_WRITE_IMPL(29);
BLYNK_ON_WRITE_IMPL(30);
BLYNK_ON_WRITE_IMPL(31);

static const WidgetReadHandler BlynkReadHandlerVector[32] BLYNK_PROGMEM = {
    BlynkWidgetRead0,   BlynkWidgetRead1,   BlynkWidgetRead2,   BlynkWidgetRead3,
    BlynkWidgetRead4,   BlynkWidgetRead5,   BlynkWidgetRead6,   BlynkWidgetRead7,
    BlynkWidgetRead8,   BlynkWidgetRead9,   BlynkWidgetRead10,  BlynkWidgetRead11,
    BlynkWidgetRead12,  BlynkWidgetRead13,  BlynkWidgetRead14,  BlynkWidgetRead15,
    BlynkWidgetRead16,  BlynkWidgetRead17,  BlynkWidgetRead18,  BlynkWidgetRead19,
    BlynkWidgetRead20,  BlynkWidgetRead21,  BlynkWidgetRead22,  BlynkWidgetRead23,
    BlynkWidgetRead24,  BlynkWidgetRead25,  BlynkWidgetRead26,  BlynkWidgetRead27,
    BlynkWidgetRead28,  BlynkWidgetRead29,  BlynkWidgetRead30,  BlynkWidgetRead31,
};

static const WidgetWriteHandler BlynkWriteHandlerVector[32] BLYNK_PROGMEM = {
    BlynkWidgetWrite0,  BlynkWidgetWrite1,  BlynkWidgetWrite2,  BlynkWidgetWrite3,
    BlynkWidgetWrite4,  BlynkWidgetWrite5,  BlynkWidgetWrite6,  BlynkWidgetWrite7,
    BlynkWidgetWrite8,  BlynkWidgetWrite9,  BlynkWidgetWrite10, BlynkWidgetWrite11,
    BlynkWidgetWrite12, BlynkWidgetWrite13, BlynkWidgetWrite14, BlynkWidgetWrite15,
    BlynkWidgetWrite16, BlynkWidgetWrite17, BlynkWidgetWrite18, BlynkWidgetWrite19,
    BlynkWidgetWrite20, BlynkWidgetWrite21, BlynkWidgetWrite22, BlynkWidgetWrite23,
    BlynkWidgetWrite24, BlynkWidgetWrite25, BlynkWidgetWrite26, BlynkWidgetWrite27,
    BlynkWidgetWrite28, BlynkWidgetWrite29, BlynkWidgetWrite30, BlynkWidgetWrite31,
};

WidgetReadHandler GetReadHandler(uint8_t pin)
{
    if (pin >= COUNT_OF(BlynkReadHandlerVector))
        return NULL;
#ifdef BLYNK_HAS_PROGMEM
    return (WidgetReadHandler)pgm_read_word(&BlynkReadHandlerVector[pin]);
#else
    return BlynkReadHandlerVector[pin];
#endif
}

WidgetWriteHandler GetWriteHandler(uint8_t pin)
{
    if (pin >= COUNT_OF(BlynkWriteHandlerVector))
        return NULL;
#ifdef BLYNK_HAS_PROGMEM
    return (WidgetWriteHandler)pgm_read_word(&BlynkWriteHandlerVector[pin]);
#else
    return BlynkWriteHandlerVector[pin];
#endif
}
