#ifdef WASM
#include "stdwasm.hpp"
#include <stdarg.h>

#define IMPORT extern "C"

IMPORT void print(char const* ptr);

//IMPORT unsigned char __heap_base;
//void* malloc(size_t size) {
//    static uint32_t bump_pointer = (uint32_t)&__heap_base;
//    uint64_t r = bump_pointer;
//    bump_pointer += size;
//    return (void*) r;
//}
//void free(void* ptr) {}


int strlen(char const* str) {
    int l = 0;
    while (*str++) ++l;
    return l;
}


int vsprintf(char *str, const char *fmt, va_list args) {
    char*       p = str;
    char const* f = fmt;
    while (*f) {
        if (*f == '%') {
            ++f;
            if (*f == '%') *p++ = '%';
            else if (*f == 's') {
                char const* s = va_arg(args, char const*);
                while (*s) *p++ = *s++;
            }
            else if (*f == 'd') {
                int d = va_arg(args, int);
                if (d < 0) {
                    d = -d;
                    *p++ = '-';
                }
                char* l = p;
                do {
                    *p++ = d % 10 + '0';
                    d /= 10;
                } while (d);
                char* r = p - 1;
                while (l < r) {
                    char c = *l;
                    *l++ = *r;
                    *r-- = c;
                }
            }
            else return -1; // error
            ++f;
        }
        else *p++ = *f++;
    }
    *p = 0;
    return p - str - 1;
}


int sprintf(char* str, char const* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(str, fmt, args);
    va_end(args);
    return n;
}


int printf(char const* fmt, ...) {
    enum { MAX_LEN = 1024 };
    char str[MAX_LEN];
    va_list args;
    va_start(args, fmt);
    int n = vsprintf(str, fmt, args);
    va_end(args);
    print(str);
    return n;
}

#endif
