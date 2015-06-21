/**
 * @file       BlynkFifo.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Feb 2015
 * @brief      FIFO implementation
 *
 */

#ifndef BlynkFifo_h
#define BlynkFifo_h

#include <utility/BlynkUtility.h>

template<typename T, unsigned SIZE>
class BlynkFifo {

    BlynkFifo(const BlynkFifo<T, SIZE> & rb);

public:

    BlynkFifo() : fst(0), lst(0), flag(0) {}
    ~BlynkFifo() {}

    void clear() {
        fst = 0;
        lst = 0;
        flag = 0;
    }

    size_t write(const T* data, size_t n)
    {
        if ((n = BlynkMin(n, getFree()))) {
            const size_t ch1 = BlynkMin(n, SIZE - lst);
            memcpy(buffer + lst, data, ch1 * sizeof(T));
            lst = (lst + ch1) % SIZE;

            if (ch1 < n) {
                const size_t ch2 = n - ch1;
                memcpy(buffer + lst, data + ch1, ch2 * sizeof(T));
                lst = (lst + ch2) % SIZE;
            }

            if (fst == lst) {
                flag = 1;
            }
        }
        return n;
    }

    size_t read(T* dest, size_t n)
    {
        if ((n = BlynkMin(n, getOccupied()))) {
            flag = 0;

            const size_t ch1 = BlynkMin(n, SIZE - fst);
            memcpy(dest, buffer + fst, ch1 * sizeof(T));
            fst = (fst + ch1) % SIZE;

            if (ch1 < n) {
                const size_t ch2 = n - ch1;
                memcpy(dest + ch1, buffer + fst, ch2 * sizeof(T));
                fst = (fst + ch2) % SIZE;
            }
        }
        return n;
    }

    bool push(const T& data) {
        return write(&data, 1) == 1;
    }

    size_t getOccupied() const {
        if (lst == fst) {
            return flag ? SIZE : 0;
        } else if (lst > fst) {
            return lst - fst;
        } else {
            return SIZE + lst - fst;
        }
    }

    size_t getFree() const {
        return SIZE - getOccupied();
    }

private:
    T buffer[SIZE];
    size_t fst;
    size_t lst;
    uint8_t flag;
};

#endif
