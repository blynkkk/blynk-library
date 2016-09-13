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
    enum TimeMode {
        TIME_UNDEFINED,
        TIME_SUNSET,
        TIME_SUNRISE,
        TIME_SPECIFIED
    };

    TimeInputParam(const BlynkParam& param)
    {
        mStartMode = TIME_UNDEFINED;
        mStopMode = TIME_UNDEFINED;
        mTZ[0] = '\0';
        mWeekdays = -1; // All set
        mTZ_Offset = 0;

        BlynkParam::iterator it = param.begin();
        if (it >= param.end())
            return;

        if (0 == strcmp(it.asStr(), "sr")) {
            mStartMode = TIME_SUNRISE;
        } else if (0 == strcmp(it.asStr(), "ss")) {
            mStartMode = TIME_SUNSET;
        } else if (!it.isEmpty()) {
            mStart = BlynkTime(it.asLong());
            if (mStart.isValid()) {
                mStartMode = TIME_SPECIFIED;
            }
        }

        if (++it >= param.end())
            return;

        if (0 == strcmp(it.asStr(), "sr")) {
            mStopMode = TIME_SUNRISE;
        } else if (0 == strcmp(it.asStr(), "ss")) {
            mStopMode = TIME_SUNSET;
        } else if (!it.isEmpty()) {
            mStop = BlynkTime(it.asLong());
            if (mStop.isValid()) {
                mStopMode = TIME_SPECIFIED;
            }
        }

        if (++it >= param.end())
            return;

        strncpy(mTZ, it.asStr(), sizeof(mTZ));

        if (++it >= param.end())
            return;

        if (!it.isEmpty()) {
            mWeekdays = 0;
            const char* p = it.asStr();

            while (int c = *p++) {
                if (c >= '1' && c <= '7') {
                    BlynkBitSet(mWeekdays, c - '1');
                }
            }
        }

        if (++it >= param.end())
            return;

        mTZ_Offset = it.asLong();
    }

    BlynkTime& getStart() { return mStart; }
    BlynkTime& getStop()  { return mStop;  }

    TimeMode getStartMode() const { return mStartMode; }
    TimeMode getStopMode()  const { return mStopMode; }

    bool hasStartTime()   const { return mStartMode == TIME_SPECIFIED; }
    bool isStartSunrise() const { return mStartMode == TIME_SUNRISE; }
    bool isStartSunset()  const { return mStartMode == TIME_SUNSET; }
    int getStartHour()    const { return mStart.hour(); }
    int getStartMinute()  const { return mStart.minute(); }
    int getStartSecond()  const { return mStart.second(); }

    bool hasStopTime()    const { return mStopMode == TIME_SPECIFIED; }
    bool isStopSunrise()  const { return mStopMode == TIME_SUNRISE; }
    bool isStopSunset()   const { return mStopMode == TIME_SUNSET; }
    int getStopHour()     const { return mStop.hour(); }
    int getStopMinute()   const { return mStop.minute(); }
    int getStopSecond()   const { return mStop.second(); }

    const char* getTZ()   const { return mTZ; }
    uint32_t getTZ_Offset() const { return mTZ_Offset; }

    bool isWeekdaySelected(int day) const {
        return BlynkBitRead(mWeekdays, (day - 1) % 7);
    }

private:
    BlynkTime mStart;
    BlynkTime mStop;
    char      mTZ[32];
    uint32_t  mTZ_Offset;

    TimeMode  mStopMode;
    TimeMode  mStartMode;

    uint8_t   mWeekdays;
};

#endif
