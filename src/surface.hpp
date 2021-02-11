#pragma once

#include "stdwasm.hpp"
#include "util.hpp"


inline uint32_t color(int r, int g, int b, int a=255) {
    return (clamp(a, 0, 255) << 24)
         | (clamp(b, 0, 255) << 16)
         | (clamp(g, 0, 255) <<  8)
         | (clamp(r, 0, 255) <<  0);
}


struct Rect {
    int x, y, w, h;
    int center_x() const { return x + w / 2; }
    int center_y() const { return y + h / 2; }
    bool overlap(Rect const& r) const {
        return !(x >= r.x + r.w || y >= r.y + r.h || x + w <= r.x || y + h <= r.y);
    }
    int overlap_x(Rect const& r) const {
        if (!overlap(r)) return 0;
        int s = r.x + r.w - x;
        int t = r.x - w - x;
        return abs(s) < abs(t) ? s : t;
    }
    int overlap_y(Rect const& r) const {
        if (!overlap(r)) return 0;
        int s = r.y + r.h - y;
        int t = r.y - h - y;
        return abs(s) < abs(t) ? s : t;
    }
};


class Surface {
public:
    void init(int w, int h);
    void init(uint8_t const* code);
    ~Surface();

    uint32_t* pixels() const { return m_pixels; }
    int width() const { return m_width; }
    int height() const { return m_height; }

    // XXX: no bounds check!
    uint32_t pixel(int x, int y) const { return m_pixels[x + y * m_width]; }
    // XXX: no bounds check!
    void pixel(int x, int y, uint32_t color) {
        m_pixels[x + y * m_width] = color;
    }

    void hline(int x1, int x2, int y, uint32_t color);
    void vline(int x, int y1, int y2, uint32_t color);
    void rect(Rect const& rect, uint32_t color);
    void rect_filled(Rect const& rect, uint32_t color);

//    enum Flip {
//        F_V = 1,
//        F_H = 2,
//    };

    void copy(Surface const& s, Rect r, int x, int y);

private:
    int       m_width  = 0;
    int       m_height = 0;
    uint32_t* m_pixels = nullptr;
};

