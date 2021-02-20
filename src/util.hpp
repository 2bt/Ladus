#pragma once

#include "stdwasm.hpp"

template <class T> T min(T const& x, T const& y) { return x < y ? x : y; }
template <class T> T max(T const& x, T const& y) { return x > y ? x : y; }
template <class T> T clamp(T const& v, T const& mi, T const& ma) { return max(mi, min(ma, v)); }
template <class T> T abs(T const& x) { return x < 0 ? -x : x; }
template <class T> T sign(T const& x) { return (x > 0) - (x < 0); }

inline int   round_to_int(float f) { return floorf(f + 0.5000f); }
inline float mix(float a, float b, float x) { return a + (b - a) * x; }
inline float smooth(float x) { return x * x * (3 - 2 * x); }

float my_sin(float x);
float my_cos(float x);

struct Random {
    enum { MAX = 32767 };
    uint32_t seed = 1;
    int rand() {
        seed = seed * 1103515245 + 12345;
        return (uint32_t)(seed / 65536) % (MAX + 1);
    }
    inline int rand_int(int a, int b) {
        return a + rand() * (b - a) / MAX;
    }
    inline float rand_float(float a, float b) {
        return a + rand() * (b - a) * (1.0f / MAX);
    }
};


int   rand_int(int a, int b);
float rand_float(float a, float b);


inline uint32_t Color(uint32_t rgb) {
    return ((rgb >> 16) & 0x0000ff)
         | ((rgb      ) & 0x00ff00)
         | ((rgb << 16) & 0xff0000)
         | 0xff000000;
}
inline uint32_t Color(int r, int g, int b, int a=255) {
    return (clamp(a, 0, 255) << 24)
         | (clamp(b, 0, 255) << 16)
         | (clamp(g, 0, 255) <<  8)
         | (clamp(r, 0, 255) <<  0);
}


struct Rect {
    int x, y, w, h;
    int left() const { return x; }
    int right() const { return x + w; }
    int top() const { return y; }
    int bottom() const { return y + h; }
    bool contains(int px, int py) const {
        return px >= x && px < right() && py >= y && py < bottom();
    }

    int center_x() const { return x + w / 2; }
    int center_y() const { return y + h / 2; }
    Rect relative(Rect const& r) const {
        return {x - r.x, y - r.y, w, h};
    }
    bool overlap(Rect const& r) const {
        return !(x >= r.right() || y >= r.bottom() || right() <= r.x || bottom() <= r.y);
    }
    int overlap_x(Rect const& r) const {
        if (!overlap(r)) return 0;
        int s = r.right() - x;
        int t = r.x - right();
        return abs(s) < abs(t) ? s : t;
    }
    int overlap_y(Rect const& r) const {
        if (!overlap(r)) return 0;
        int s = r.bottom() - y;
        int t = r.y - bottom();
        return abs(s) < abs(t) ? s : t;
    }
};
