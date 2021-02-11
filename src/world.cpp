#include "world.hpp"
#include "actor.hpp"
#include "vector.hpp"
#include "app.hpp"


namespace {


struct Room {
    char const* data;
};


void load_room(Room const& room) {
    using namespace world;

    player = new Player;
    actors.append(player);

    char c;
    for (int i = 0; (c = room.data[i]) ; ++i) {
        if (c == '.') continue;
        int x = i % ROOM_WIDTH * TILE_SIZE;
        int y = i / ROOM_WIDTH * TILE_SIZE;

        if (c == '0') {
            solids.append(new Solid(x, y));
        }
        if (c == '^') {
            solids.append(new Solid(x, y, true));
        }

        else if (c == '@') {
            player->init(x + TILE_SIZE / 2, y + TILE_SIZE);
        }
    }
}


} // namespace


Player*        world::player;
Vector<Solid*> world::solids;
Vector<Actor*> world::actors;


void world::init() {
    load_room({
        "000000................00"
        "0.....................00"
        "0.....................00"
        "0.....................00"
        "0.............0^^^000000"
        "0........^^^..0...0....."
        "0.............0...0....."
        "0000^^^................."
        "........................"
        "........00.............."
        "....@...00.............."
        "00000000000000000^^^0000"
        "00000000000000000...0000"
        "00000000000000000...0000"
    });
}


void world::update() {

    for (Solid* s : solids) s->update();

    int j = 0;
    for (int i = 0; i < actors.len(); ++i) {
        Actor* a = actors[i];
        a->update();
        if (a->alive()) actors[j++] = a;
        else delete a;
    }
    actors.shrink(j);

}

void world::draw() {
    for (Solid* s : solids) s->draw();
    for (Actor* a : actors) a->draw();
}
