#pragma once

#include "util.hpp"


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

