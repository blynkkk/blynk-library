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

class TimeInputParam
{
public:

	TimeInputParam(const BlynkParam& param)
	{
		if (strlen(param[0].asStr())) {
			mStart = param[0].asLong();
		}
		if (strlen(param[1].asStr())) {
			mStop = param[1].asLong();
		}
		mTZ = param[2].asLong();
	}

	BlynkTime& getStart() { return mStart; }
	BlynkTime& getStop()  { return mStop;  }
    long getTZ()  const { return mTZ; }

private:
    BlynkTime mStart;
    BlynkTime mStop;
    long mTZ;
};

#endif
