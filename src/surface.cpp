#include "surface.hpp"


Surface::~Surface() {
    free(m_pixels);
}

void Surface::init(int w, int h) {
    m_width  = w;
    m_height = h;
    m_pixels = (uint32_t*) malloc(w * h * sizeof(uint32_t));
}

void Surface::init(uint8_t const* code) {
    init(*(uint16_t*) code, *(uint16_t*) (code + 2));
    struct {
        uint8_t const* p;
        int i = 0;
        uint32_t operator()(int nbits) {
            uint32_t v = 0;
            while (nbits > 0) {
                int b = min(8 - i, nbits);
                nbits -= b;
                v <<= b;
                v |= (*p >> i) & ((1 << b) - 1);
                i += b;
                p += i == 8;
                i &= 7;
            }
            return v;
        }
    } read{code + 5 + code[4] * 4};

    enum {
        MIN_L   = 3,
        BITS_C  = 8,
        BITS_D  = 9,
        BITS_L1 = 4,
        BITS_L2 = 7,
    };

    uint32_t* p = m_pixels;
    uint32_t* end = p + m_width * m_height;
    while (p < end) {
        int d = read(BITS_D);
        if (d == 0) {
            *p++ = ((uint32_t*) (code + 5))[read(BITS_C)];
        }
        else {
            int l = read(read(1) ? BITS_L2 : BITS_L1) + MIN_L;
            for (int i = 0; i < l; ++i, ++p) *p = p[-d];
        }
    }
}

void Surface::hline(int x1, int x2, int y, uint32_t color) {
    if (y < 0 || y >= m_height) return;
    x1 = max(0, x1);
    x2 = min(m_width, x2);
    for (int x = x1; x < x2; ++x) pixel(x, y, color);
}

void Surface::vline(int x, int y1, int y2, uint32_t color) {
    if (x < 0 || x >= m_width) return;
    y1 = max(0, y1);
    y2 = min(m_height, y2);
    for (int y = y1; y < y2; ++y) pixel(x, y, color);
}

void Surface::rect(Rect const& rect, uint32_t color) {
    hline(rect.x, rect.x + rect.w, rect.y, color);
    hline(rect.x, rect.x + rect.w, rect.y + rect.h - 1, color);
    vline(rect.x, rect.y, rect.y + rect.h, color);
    vline(rect.x + rect.w - 1, rect.y, rect.y + rect.h, color);
}

void Surface::rect_filled(Rect const& rect, uint32_t color) {
    int x1 = max(0, rect.x);
    int x2 = min(m_width, rect.x + rect.w);
    int y1 = max(0, rect.y);
    int y2 = min(m_height, rect.y + rect.h);
    for (int x = x1; x < x2; ++x)
    for (int y = y1; y < y2; ++y) pixel(x, y, color);
}

void Surface::copy(Surface const& s, Rect r, int x, int y) {
    if (x < 0) {
        r.w += x;
        r.x -= x;
        x = 0;
    }
    if (r.w > m_width - x) r.w = m_width - x;
    if (y < 0) {
        r.h += y;
        r.y -= y;
        y = 0;
    }
    if (r.h > m_height - y) r.h = m_height - y;
    for (int yy = 0; yy < r.h; ++yy)
    for (int xx = 0; xx < r.w; ++xx) {
        uint32_t c = s.pixel(r.x + xx, r.y + yy);
        if (c & 0xff000000) pixel(x + xx, y + yy, c);
    }
}
