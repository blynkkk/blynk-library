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
    	if (0 == strcmp(param[0].asStr(), "sr")) {
    		mStartMode = TIME_SUNRISE;
		} else if (0 == strcmp(param[0].asStr(), "ss")) {
    		mStartMode = TIME_SUNSET;
		} else if (!param[0].isEmpty()) {
			mStart = BlynkTime(param[0].asLong());
			if (mStart.isValid()) {
				mStartMode = TIME_SPECIFIED;
			}
		}

    	mStopMode = TIME_UNDEFINED;
    	if (0 == strcmp(param[1].asStr(), "sr")) {
    		mStopMode = TIME_SUNRISE;
		} else if (0 == strcmp(param[1].asStr(), "ss")) {
			mStopMode = TIME_SUNSET;
		} else if (!param[1].isEmpty()) {
			mStop = BlynkTime(param[1].asLong());
			if (mStop.isValid()) {
				mStopMode = TIME_SPECIFIED;
			}
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

    long getTZ()  const { return mTZ; }

    bool isWeekdaySelected(int day) const {
        return BlynkBitRead(mWeekdays, (day - 1) % 7);
    }

private:
    BlynkTime mStart;
    BlynkTime mStop;
    int32_t   mTZ;

    TimeMode  mStopMode;
    TimeMode  mStartMode;

    uint8_t   mWeekdays;
};

#endif
