/**
 * @file       WidgetTimeInput.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Aug 2016
 * @brief
 *
 */

#ifndef WidgetTimeInput_h
#define WidgetTimeInput_h

#include <Blynk/BlynkApi.h>
#include <utility/BlynkDateTime.h>
#include <utility/BlynkUtility.h>

class TimeInputParam
{
public:

    TimeInputParam(const BlynkParam& param)
    {
        if (!param[0].isEmpty()) {
            mStart = param[0].asLong();
        }
        if (!param[1].isEmpty()) {
            mStop = param[1].asLong();
        }
        mTZ = param[2].asLong();

        if (param[3].isEmpty()) {
			mWeekdays = -1; // All set
        } else {
			mWeekdays = 0;
			const char* p = param[3].asStr();

			while (int c = *p++) {
				if (c >= '1' && c <= '7') {
					BlynkBitSet(mWeekdays, c - '1');
				}
			}
        }
    }

    BlynkTime& getStart() { return mStart; }
    BlynkTime& getStop()  { return mStop;  }
    long getTZ()  const { return mTZ; }

    bool isWeekdaySelected(int day) const {
        return BlynkBitRead(mWeekdays, (day - 1) % 7);
    }

private:
    BlynkTime mStart;
    BlynkTime mStop;
    int32_t   mTZ;
    uint8_t   mWeekdays;
};

#endif
