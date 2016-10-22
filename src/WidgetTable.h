/**
 * @file       WidgetTable.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Sep 2016
 * @brief
 *
 */

#ifndef WidgetTable_h
#define WidgetTable_h

#include <Blynk/BlynkApi.h>

class WidgetTable
{
public:
    typedef void (*ItemSelectChange)(int index, bool selected);
    typedef void (*ItemOrderChange)(int indexFrom, int indexTo);

public:
    WidgetTable(uint8_t pin = -1)
        : mPin(pin)
        , mOnOrderChange(NULL)
        , mOnSelectChange(NULL)
    {}

    void setVPin(int vPin) { mPin = vPin; }

    void onWrite(BlynkReq BLYNK_UNUSED &request, const BlynkParam& param) {
        if (mOnOrderChange && 0 == strcmp(param[0].asStr(), "order")) {
            mOnOrderChange(param[1].asInt(), param[2].asInt());
        } else if (mOnSelectChange && 0 == strcmp(param[0].asStr(), "select")) {
            mOnSelectChange(param[1].asInt(), true);
        } else if (mOnSelectChange && 0 == strcmp(param[0].asStr(), "deselect")) {
            mOnSelectChange(param[1].asInt(), false);
        }
    }

    void onOrderChange(ItemOrderChange cbk)   { mOnOrderChange = cbk; }
    void onSelectChange(ItemSelectChange cbk) { mOnSelectChange = cbk; }

    void clear() {
        Blynk.virtualWrite(mPin, "clr");
    }

    template <typename T1, typename T2>
    void addRow(int index, const T1& name, const T2& value) {
        Blynk.virtualWrite(mPin, "add", index, name, value);
    }

    void pickRow(int index) {
        Blynk.virtualWrite(mPin, "pick", index);
    }

private:
    uint8_t mPin;
    ItemOrderChange  mOnOrderChange;
    ItemSelectChange mOnSelectChange;

};

#endif
