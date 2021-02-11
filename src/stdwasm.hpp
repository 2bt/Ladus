#pragma once

#ifdef WASM

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

void* malloc(size_t size);
void  free(void* ptr);

int   strlen(char const* str);
int   vsprintf(char* str, const char* fmt, va_list args);
int   sprintf(char* str, char const* fmt, ...);
int   printf(char const* fmt, ...);

#define assert(x) ((void)0)
#else

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cassert>

#endif
