#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkHelpers.h>

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

#if !defined(BLYNK_NO_LONGLONG)

char* lltoa_internal(long long val, char* buf, unsigned buf_len, int base)
{
    int i = buf_len-2;
    int sign = (val < 0);
    buf[buf_len-1] = '\0';

    if (val == 0) {
        buf[i] = '0';
        return &buf[i];
    }

    unsigned long long absval = sign ? -val : val;

    for (; absval && i ; --i, absval /= base) {
        buf[i] = "0123456789abcdef"[absval % base];
    }

    if (sign) { buf[i--] = '-'; }

    return &buf[i+1];
}

char* ulltoa_internal(unsigned long long val, char* buf, unsigned buf_len, int base)
{
    int i = buf_len-2;
    buf[buf_len-1] = '\0';

    if (val == 0) {
        buf[i] = '0';
        return &buf[i];
    }

    for (; val && i ; --i, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }

    return &buf[i+1];
}

#endif

