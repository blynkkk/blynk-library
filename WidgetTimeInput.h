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

	TimeInputParam(BlynkParam& param)
		: mStart (param[0].asLong())
		, mStop  (param[1].asLong())
	{
		mTZ = param[2].asLong();
	}

	BlynkDateTime& getStart() { return mStart; }
	BlynkDateTime& getStop()  { return mStop;  }
    long getTZ()  const { return mTZ; }

private:
    BlynkDateTime mStart;
    BlynkDateTime mStop;
    long mTZ;
};

#endif
