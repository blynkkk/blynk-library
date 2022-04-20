/**
 * @file       BlynkDateTime.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Aug 2016
 * @brief      DateTime implementation
 *
 */

#ifndef BlynkDateTime_h
#define BlynkDateTime_h

#include <stdint.h>

typedef int64_t blynk_time_t;

struct blynk_tm *blynk_gmtime_r(const blynk_time_t *time, struct blynk_tm *tm);
blynk_time_t blynk_mk_gmtime(struct blynk_tm *tm);
int blynk_compute_sun(int8_t month, int8_t day, double latitude, double longitude, bool rise);

static inline
bool isTimeValid(blynk_time_t t_sec) {
    return t_sec > 1609459200; // 01 Jan 2021
}

struct blynk_tm {
    int8_t          tm_sec;     // seconds after the minute     0-59 (max 61)
    int8_t          tm_min;     // minutes after the hour       0-59
    int8_t          tm_hour;    // minutes after the hour       0-23
    int8_t          tm_mday;    // day of the month             1-31
    int8_t          tm_wday;    // days since Sunday            0-6
    int8_t          tm_mon;     // months since January         0-11
    int16_t         tm_year;    // years since 1900
    int16_t         tm_yday;    // days since January 1         0-365
};

#define BLYNK_SECS_PER_MIN     (60UL)
#define BLYNK_SECS_PER_HOUR    (3600UL)
#define BLYNK_SECS_PER_DAY     (BLYNK_SECS_PER_HOUR * 24UL)
#define BLYNK_SECS_PER_WEEK    (BLYNK_SECS_PER_DAY * 7UL)

static const char* DOW_STR[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

class BlynkTime {

public:
    static BlynkTime invalid() { return BlynkTime(); }

    static BlynkTime computeSunRise(int8_t month, int8_t day, double latitude, double longitude) {
        int minutes = blynk_compute_sun(month, day, latitude, longitude,  true);
        if (minutes < 0) return invalid();
        return BlynkTime(0, minutes, 0);
    }

    static BlynkTime computeSunSet(int8_t month, int8_t day, double latitude, double longitude) {
        int minutes = blynk_compute_sun(month, day, latitude, longitude, false);
        if (minutes < 0) return invalid();
        return BlynkTime(0, minutes, 0);
    }


    BlynkTime() : mTime(-1) {}

    BlynkTime(const BlynkTime& t) : mTime(t.mTime) {}

    BlynkTime(long seconds) : mTime(seconds % BLYNK_SECS_PER_DAY) {}

    BlynkTime(int hour, int minute, int second)
    {
        mTime = (hour * BLYNK_SECS_PER_HOUR + minute * BLYNK_SECS_PER_MIN + second) % BLYNK_SECS_PER_DAY;
    }

    int second() const { return (mTime % BLYNK_SECS_PER_MIN); }
    int minute() const { return (mTime / BLYNK_SECS_PER_MIN) % BLYNK_SECS_PER_MIN; }
    int hour()   const { return (mTime / BLYNK_SECS_PER_HOUR); }

    int hour12() const {
        int h = hour();
        if (h == 0)
            return 12; // 12 midnight
        else if (h > 12)
            return h - 12;
        return h;
    }

    bool isAM() const { return !isPM(); }
    bool isPM() const { return (hour() >= 12); }

    void adjustSeconds(int sec) {
        if (isValid()) {
            mTime = (mTime + sec) % BLYNK_SECS_PER_DAY;
        }
    }

    blynk_time_t getUnixOffset() const { return mTime; }

    bool isValid()  const { return mTime < BLYNK_SECS_PER_DAY; }
    operator bool() const { return isValid(); }

    bool operator != (const BlynkTime& t) const { return mTime != t.mTime; }
    bool operator == (const BlynkTime& t) const { return mTime == t.mTime; }
    bool operator >= (const BlynkTime& t) const { return mTime >= t.mTime; }
    bool operator <= (const BlynkTime& t) const { return mTime <= t.mTime; }
    bool operator >  (const BlynkTime& t) const { return mTime >  t.mTime; }
    bool operator <  (const BlynkTime& t) const { return mTime <  t.mTime; }

private:
    uint32_t mTime;
};

class BlynkDateTime {

public:
    static BlynkDateTime invalid() { return BlynkDateTime(); }

    BlynkDateTime() : mTime(0) {}

    BlynkDateTime(const BlynkDateTime& t)
    {
        mTime = t.mTime;
        blynk_gmtime_r(&mTime, &mTm);
    }

    BlynkDateTime(blynk_time_t t)
    {
        mTime = t;
        blynk_gmtime_r(&mTime, &mTm);
    }

    BlynkDateTime(int hour, int minute, int second, int day, int month, int year)
    {
        mTm.tm_hour = hour;
        mTm.tm_min  = minute;
        mTm.tm_sec  = second;

        mTm.tm_mday = day;
        mTm.tm_mon  = month - 1;
        mTm.tm_year = year - 1900;

        mTime = blynk_mk_gmtime(&mTm);
    }

    BlynkDateTime(const BlynkTime& time, int day, int month, int year)
        : BlynkDateTime(time.hour(), time.minute(), time.second(), day, month, year)
    {}

    BlynkDateTime(const BlynkTime& time, const BlynkDateTime& date)
        : BlynkDateTime(time.hour(), time.minute(), time.second(), date.day(), date.month(), date.year())
    {}

    int second() const { return mTm.tm_sec; }
    int minute() const { return mTm.tm_min; }
    int hour()   const { return mTm.tm_hour; }
    int day()    const { return mTm.tm_mday; }
    int month()  const { return 1 + mTm.tm_mon; }
    int year()   const { return 1900 + mTm.tm_year; }

    int yearday() const { return 1 + mTm.tm_yday; } // 1 = Jan 1, 2 = Jan 2 ...
    int weekday() const { return mTm.tm_wday; }     // 0 = Sun,   1 = Mon ...

    // Deprecated: // 1 = Mon, ..., 7 = Sun
    int day_of_week() const { return mTm.tm_wday == 0 ? 7 : mTm.tm_wday; }
    int day_of_year() const { return 1 + mTm.tm_yday; }
    const char* dow_str() const { return DOW_STR[mTm.tm_wday % 7]; }

    int weak_of_year() const {
        int julian = yearday();
        int dow = mTm.tm_wday;
        int dowJan1 = BlynkDateTime(0,0,0, 1,1,year()).weekday();
        int weekNum = ((julian + 6) / 7);
        if (dow < dowJan1) {
            ++weekNum;
        }
        return (weekNum);
    }

    int getSecsToday() const                { return mTime % BLYNK_SECS_PER_DAY; }
    int getSecsThisWeek() const             { return (mTm.tm_wday * BLYNK_SECS_PER_DAY) + getSecsToday(); }
    BlynkDateTime getPrevMidnight() const   { return BlynkDateTime((mTime / BLYNK_SECS_PER_DAY) * BLYNK_SECS_PER_DAY); }
    BlynkDateTime getNextMidnight() const   { return BlynkDateTime((mTime / BLYNK_SECS_PER_DAY) * BLYNK_SECS_PER_DAY + BLYNK_SECS_PER_DAY); }
    BlynkDateTime getPrevSunday() const     { return BlynkDateTime(mTime - getSecsThisWeek()); }
    BlynkDateTime getNextSunday() const     { return BlynkDateTime(mTime - getSecsThisWeek() + BLYNK_SECS_PER_WEEK); }

    int hour12() const {
        int h = hour();
        if (h == 0)
            return 12; // 12 midnight
        else if (h > 12)
            return h - 12;
        return h;
    }

    bool isAM() const { return !isPM(); }
    bool isPM() const { return (hour() >= 12); }

    void adjustSeconds(int sec) {
        if (isValid()) {
            mTime += sec;
            blynk_gmtime_r(&mTime, &mTm);
        }
    }

    //tm& getTm() { return mTm; }
    blynk_time_t getUnix() const { return mTime; }

    bool isValid()  const { return mTime != 0; }
    operator blynk_time_t() const { return mTime; }

    bool operator != (const BlynkDateTime& t) const { return mTime != t.mTime; }
    bool operator == (const BlynkDateTime& t) const { return mTime == t.mTime; }
    bool operator >= (const BlynkDateTime& t) const { return mTime >= t.mTime; }
    bool operator <= (const BlynkDateTime& t) const { return mTime <= t.mTime; }
    bool operator >  (const BlynkDateTime& t) const { return mTime >  t.mTime; }
    bool operator <  (const BlynkDateTime& t) const { return mTime <  t.mTime; }

private:
    blynk_tm        mTm;
    blynk_time_t    mTime;
};

#endif
