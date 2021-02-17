#include <stdarg.h>
#include "app.hpp"
#include "vector.hpp"
#include "surfaces.hpp"
#include "world.hpp"


namespace {

int     g_tick = 999;

} // namespace


Surface app::screen;
Input   app::input;


void update_input(uint32_t bits) {
    using namespace app;
    enum {
        LEFT  = 1,
        RIGHT = 2,
        UP    = 4,
        DOWN  = 8,
        JUMP  = 16,
        SHOOT = 32,
    };
    input.prev_x     = input.x;
    input.prev_y     = input.y;
    input.prev_jump  = input.jump;
    input.prev_shoot = input.shoot;
    input.x     = !!(bits & RIGHT) - !!(bits & LEFT);
    input.y     = !!(bits & DOWN ) - !!(bits & UP  );
    input.jump  = !!(bits & JUMP);
    input.shoot = !!(bits & SHOOT);
}


void app::init() {
    screen.init(WIDTH, HEIGHT);
    surfaces::init();
    world::init();
}


void app::update(uint32_t input_bits) {
    ++g_tick;
    update_input(input_bits);

    world::update();
}


void app::draw() {

    screen.rect_filled({0, 0, WIDTH, HEIGHT}, 0);
//    for (int y = 0; y < HEIGHT; ++y)
//    for (int x = 0; x < WIDTH; ++x) {
//        int b = ((x + g_tick / 2) / 12 % 31 ^ (y + g_tick / 2) / 12 % 29) % 11 % 3;
//        screen.pixel(x, y, Color(10, 7 + b * 10,  b * 15));
//    }

    world::draw();
}


void app::print(int x, int y, char const* fmt, ...) {
    enum { MAX_LEN = 1024 };
    char str[MAX_LEN];
    va_list args;
    va_start(args, fmt);
    vsprintf(str, fmt, args);
    va_end(args);
    int xx = x;
    int c;
    for (int i = 0; (c = str[i]); ++i) {
        if (c == '\n') {
            y += 12;
            xx = x;
            continue;
        }
        if (c > 32) {
            c -= 32;
            screen.copy(surfaces::font, {c % 32 * 8, c / 32 * 8, 8, 8}, xx, y);
        }
        xx += 8;
    }
}
