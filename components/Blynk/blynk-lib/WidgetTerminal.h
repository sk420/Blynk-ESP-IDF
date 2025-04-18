/**
 * @file       WidgetTerminal.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 */

#ifndef WidgetTerminal_h
#define WidgetTerminal_h

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TERMINAL_PRINT_BUF_SIZE 128

#include <Blynk/BlynkWidgetBase.h>



class WidgetTerminal
    : public BlynkWidgetBase
{
public:
    WidgetTerminal(uint8_t vPin)
        : BlynkWidgetBase(vPin)
        , mOutQty(0)
    {}

    //virtual ~WidgetTerminal() {}

    virtual size_t write(uint8_t byte) {
        mOutBuf[mOutQty++] = byte;
        if (mOutQty >= sizeof(mOutBuf)) {
            flush();
        }
        return 1;
    }

    virtual void flush() {
        if (mOutQty) {
            Blynk.virtualWriteBinary(mPin, mOutBuf, mOutQty);
            mOutQty = 0;
        }
    }
    
    void clear() {
        flush();
        Blynk.virtualWrite(mPin, "clr");
    }

    virtual size_t write(const void* buff, size_t len) {
        uint8_t* buf = (uint8_t*)buff;
        size_t left = len;
        while (left--) {
            write(*buf++);
        }
        return len;
    }

    virtual size_t write(const char* str) {
        return write(str, strlen(str));
    }

    // Basic C-style printf
    // void print(const char* fmt, ...) {
    //     char buf[TERMINAL_PRINT_BUF_SIZE];
    //     va_list args;
    //     va_start(args, fmt);
    //     vsnprintf(buf, sizeof(buf), fmt, args);
    //     va_end(args);
    //     write(buf, strlen(buf));
    // }

    // void println(const char* fmt, ...) {
    //     char buf[TERMINAL_PRINT_BUF_SIZE];
    //     va_list args;
    //     va_start(args, fmt);
    //     vsnprintf(buf, sizeof(buf) - 3, fmt, args);  // Room for \r\n
    //     va_end(args);
    //     strcat(buf, "\r\n");
    //     write(buf, strlen(buf));
    // }

    // Overloads (mimic Arduino behavior)
    size_t print(const char c)               { return write(&c, 1); }
    size_t print(const char str[])           { return write(str, strlen(str)); }
    size_t print(int value, int base = 10)   { return printNumber((long)value, base); }
    size_t print(unsigned int value, int base = 10) { return printNumber((unsigned long)value, base); }
    size_t print(long value, int base = 10)  { return printNumber(value, base); }
    size_t print(unsigned long value, int base = 10) { return printNumber(value, base); }
    size_t print(double value, int digits = 2) { return printFloat(value, digits); }
    size_t print(const std::string& str) {return write(str.c_str(), str.length());}


    size_t println() { return write("\r\n", 2); }
    size_t println(const char c) { size_t n = print(c); n += println(); return n; }
    size_t println(const char str[]) { size_t n = print(str); n += println(); return n; }
    size_t println(int value, int base = 10) { size_t n = print(value, base); n += println(); return n; }
    size_t println(unsigned int value, int base = 10) { size_t n = print(value, base); n += println(); return n; }
    size_t println(long value, int base = 10) { size_t n = print(value, base); n += println(); return n; }
    size_t println(unsigned long value, int base = 10) { size_t n = print(value, base); n += println(); return n; }
    size_t println(double value, int digits = 2) { size_t n = print(value, digits); n += println(); return n; }
    size_t println(const std::string& str) { size_t n = write(str.c_str(), str.length()); n += println(); return n;}

private:
    uint8_t mOutBuf[64];
    uint8_t mOutQty;

    // Custom implementation of ultoa (unsigned long to ascii)
    void ultoa(unsigned long n, char* str, int base) {
        char temp[33];
        int i = 0;
        
        if (n == 0) {
            temp[i++] = '0';
        } else {
            while (n) {
                int rem = n % base;
                temp[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
                n = n / base;
            }
        }

        // Reverse the string
        int j = 0;
        while (i > 0) {
            str[j++] = temp[--i];
        }
        str[j] = '\0';  // Null-terminate the string
    }

    size_t printNumber(unsigned long n, uint8_t base) {
        char buf[33];
        ultoa(n, buf, base);
        return write(buf, strlen(buf));
    }

    size_t printNumber(long n, uint8_t base) {
        if (n < 0) {
            write("-", 1);
            return 1 + printNumber((unsigned long)(-n), base);
        }
        return printNumber((unsigned long)n, base);
    }

    size_t printFloat(double number, uint8_t digits) {
        char buf[TERMINAL_PRINT_BUF_SIZE];
        snprintf(buf, sizeof(buf), "%.*f", digits, number);
        return write(buf, strlen(buf));
    }
};

#endif
