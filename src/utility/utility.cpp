#include <Blynk/BlynkDebug.h>
#include <Blynk/BlynkDateTime.h>

#include <math.h>

#if !defined(BLYNK_NO_FLOAT) && defined(BLYNK_USE_INTERNAL_DTOSTRF)

#include <string.h>
#include <stdio.h>

char* dtostrf_internal(double number, signed char BLYNK_UNUSED width, unsigned char prec, char *s) {
    if(isnan(number)) {
        strcpy(s, "nan");
        return s;
    }
    if(isinf(number)) {
        strcpy(s, "inf");
        return s;
    }

    if(number > 4294967040.0 || number < -4294967040.0) {
        strcpy(s, "ovf");
        return s;
    }
    char* out = s;
    // Handle negative numbers
    if(number < 0.0) {
        *out = '-';
        ++out;
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for(uint8_t i = 0; i < prec; ++i) {
        rounding /= 10.0;
    }

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long) number;
    double remainder = number - (double) int_part;
    out += sprintf(out, "%lu", int_part);

    // Print the decimal point, but only if there are digits beyond
    if(prec > 0) {
        *out++ = '.';
    }

    while(prec-- > 0) {
        remainder *= 10.0;
        if((int)remainder == 0) {
            *out++ = '0';
        }
    }
    if((int)remainder != 0) {
        sprintf(out, "%d", (int)remainder);
    } else {
        *out++ = '\0';
    }

    return s;
}

#endif

#if !defined(BLYNK_NO_LONGLONG) && defined(BLYNK_USE_INTERNAL_ATOLL)

long long atoll_internal(const char *instr)
{
    long long retval = 0;
    for (; *instr; instr++) {
        retval = 10*retval + (*instr - '0');
    }
    return retval;
}

#endif

#define YEAR_0                  1900
#define YEAR_EPOCH              1970
#define SECS_IN_DAY             (24L * 60L * 60L)
#define IS_LEAP_YEAR(year)      (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define YEAR_DAYS(year)         (IS_LEAP_YEAR(year) ? 366 : 365)

#define TIME_MAX                2147483647L

static const int month_tab[2][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

struct blynk_tm* blynk_gmtime_r(const blynk_time_t* t, struct blynk_tm *tm)
{
    blynk_time_t time = *t;
    unsigned long dayclock, dayno;
    int year = YEAR_EPOCH;

    dayclock = (unsigned long) time % SECS_IN_DAY;
    dayno = (unsigned long) time / SECS_IN_DAY;

    tm->tm_sec = dayclock % 60;
    tm->tm_min = (dayclock % 3600) / 60;
    tm->tm_hour = dayclock / 3600;
    tm->tm_wday = (dayno + 4) % 7;
    while (dayno >= (unsigned long) YEAR_DAYS(year)) {
        dayno -= YEAR_DAYS(year);
        year++;
    }
    tm->tm_year = year - YEAR_0;
    tm->tm_yday = dayno;
    tm->tm_mon = 0;
    while (dayno >= (unsigned long) month_tab[IS_LEAP_YEAR(year)][tm->tm_mon]) {
        dayno -= month_tab[IS_LEAP_YEAR(year)][tm->tm_mon];
        tm->tm_mon++;
    }
    tm->tm_mday = dayno + 1;
    return tm;
}

blynk_time_t blynk_mk_gmtime(struct blynk_tm *tm)
{
    long day, year;
    int tm_year;
    int yday, month;
    long seconds;
    int overflow;

    tm->tm_min += tm->tm_sec / 60;
    tm->tm_sec %= 60;
    if (tm->tm_sec < 0) {
        tm->tm_sec += 60;
        tm->tm_min--;
    }
    tm->tm_hour += tm->tm_min / 60;
    tm->tm_min = tm->tm_min % 60;
    if (tm->tm_min < 0) {
        tm->tm_min += 60;
        tm->tm_hour--;
    }
    day = tm->tm_hour / 24;
    tm->tm_hour = tm->tm_hour % 24;
    if (tm->tm_hour < 0) {
        tm->tm_hour += 24;
        day--;
    }
    tm->tm_year += tm->tm_mon / 12;
    tm->tm_mon %= 12;
    if (tm->tm_mon < 0) {
        tm->tm_mon += 12;
        tm->tm_year--;
    }
    day += (tm->tm_mday - 1);
    while (day < 0) {
        if (--tm->tm_mon < 0) {
            tm->tm_year--;
            tm->tm_mon = 11;
        }
        day += month_tab[IS_LEAP_YEAR(YEAR_0 + tm->tm_year)][tm->tm_mon];
    }
    while (day >= month_tab[IS_LEAP_YEAR(YEAR_0 + tm->tm_year)][tm->tm_mon]) {
        day -= month_tab[IS_LEAP_YEAR(YEAR_0 + tm->tm_year)][tm->tm_mon];
        if (++(tm->tm_mon) == 12) {
            tm->tm_mon = 0;
            tm->tm_year++;
        }
    }
    tm->tm_mday = day + 1;
    year = YEAR_EPOCH;
    if (tm->tm_year < year - YEAR_0)
        return (blynk_time_t) -1;
    seconds = 0;
    day = 0;
    overflow = 0;

    tm_year = tm->tm_year + YEAR_0;

    if (TIME_MAX / 365 < tm_year - year)
        overflow++;
    day = (tm_year - year) * 365;
    if (TIME_MAX - day < (tm_year - year) / 4 + 1)
        overflow++;
    day += (tm_year - year) / 4 + ((tm_year % 4) && tm_year % 4 < year % 4);
    day -= (tm_year - year) / 100
            + ((tm_year % 100) && tm_year % 100 < year % 100);
    day += (tm_year - year) / 400
            + ((tm_year % 400) && tm_year % 400 < year % 400);

    yday = month = 0;
    while (month < tm->tm_mon) {
        yday += month_tab[IS_LEAP_YEAR(tm_year)][month];
        month++;
    }
    yday += (tm->tm_mday - 1);
    if (day + yday < 0)
        overflow++;
    day += yday;

    tm->tm_yday = yday;
    tm->tm_wday = (day + 4) % 7;

    seconds = ((tm->tm_hour * 60L) + tm->tm_min) * 60L + tm->tm_sec;

    if ((TIME_MAX - seconds) / SECS_IN_DAY < day)
        overflow++;
    seconds += day * SECS_IN_DAY;

    if (overflow)
        return (blynk_time_t) -1;

    if ((blynk_time_t) seconds != seconds)
        return (blynk_time_t) -1;
    return (blynk_time_t) seconds;
}

int blynk_compute_sun(int8_t month, int8_t day, double latitude, double longitude, bool rise)
{
    float y, decl, eqt, ha, lon, lat;
    uint8_t a;
    int minutes;

    month -= 1;
    day   -= 1;
    lat   =   latitude/57.295779513082322;
    lon   = -longitude/57.295779513082322;

    //approximate hour;
    a = (rise) ? 18 : 6;

    // approximate day of year
    y = month * 30.4375 + day  + a/24.0; // 0... 365

    // compute fractional year
    y *= 1.718771839885e-02; // 0... 1

    // compute equation of time... .43068174
    eqt = 229.18 * (0.000075+0.001868*cos(y)  -0.032077*sin(y) -0.014615*cos(y*2) -0.040849*sin(y* 2) );

    // compute solar declination... -0.398272
    decl = 0.006918-0.399912*cos(y)+0.070257*sin(y)-0.006758*cos(y*2)+0.000907*sin(y*2)-0.002697*cos(y*3)+0.00148*sin(y*3);

    //compute hour angle
    ha = (  cos(1.585340737228125) / (cos(lat)*cos(decl)) -tan(lat) * tan(decl)  );

    if (fabs(ha) > 1.0) { // we're in the (ant)arctic and there is no rise(or set) today!
        return -1;
    }

    ha = acos(ha);
    if(!rise) ha = -ha;

    // compute minutes from midnight
    minutes = 720+4*(lon-ha)*57.295779513082322-eqt;

    return minutes;
}

