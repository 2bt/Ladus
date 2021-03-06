#pragma once

#include "surface.hpp"

struct Input {
    int x, y;
    int prev_x, prev_y;
    bool jump, shoot;
    bool prev_jump, prev_shoot;

    bool jump_just_pressed() const { return jump && !prev_jump; }
    bool jump_just_released() const { return !jump && prev_jump; }
};


namespace app {

    enum {
//        WIDTH  = 320,
//        HEIGHT = 180,
        WIDTH  = 384,
        HEIGHT = 216,
    };

    extern Surface screen;
    extern Input input;

    void init();
    void update(uint32_t input_bits);
    void draw();

    void print(int x, int y, char const* fmt, ...);
}
