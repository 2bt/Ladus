#pragma once

#include "stdwasm.hpp"

template <class T> T min(T const& x, T const& y) { return x < y ? x : y; }
template <class T> T max(T const& x, T const& y) { return x > y ? x : y; }
template <class T> T clamp(T const& v, T const& mi, T const& ma) { return max(mi, min(ma, v)); }
template <class T> T abs(T const& x) { return x < 0 ? -x : x; }
template <class T> T sign(T const& x) { return (x > 0) - (x < 0); }

void* operator new(size_t size);
void operator delete(void* ptr);

#ifdef RAND_MAX
#undef RAND_MAX
#endif

enum {
    RAND_MAX = 32767
};

int  rand();
void srand(uint32_t seed);

inline int rand_int(int a, int b) {
    return a + rand() * (b - a) / RAND_MAX;
}
inline float rand_float(float a, float b) {
    return a + rand() * (b - a) * (1.0f / RAND_MAX);
}
