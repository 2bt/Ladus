#pragma once

#include "hero.hpp"
#include "solid.hpp"
#include "vector.hpp"

enum {
    TILE_SIZE  = 16,
    ROOM_WIDTH = 24,
};

namespace world {

    extern Rect           camera;
    extern Hero*          hero;
    extern Vector<Actor*> actors;
    extern Vector<Solid*> solids;

    void init();
    void update();
    void draw();
}
