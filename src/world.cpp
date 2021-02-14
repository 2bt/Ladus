#include "world.hpp"
#include "actor.hpp"
#include "foe.hpp"
#include "vector.hpp"
#include "app.hpp"


using namespace world;
namespace {


struct Room {
    Rect rect;
    char const* data;

    char tile(int c, int r) const {
        if (c < 0 || c >= rect.w) return '0';
        if (r < 0 || r >= rect.h) return '0';
        return data[r * rect.w + c];
    }
};


Room g_rooms[] = {
    {
        {0, 0, 24, 14},
        "000000000000000000000000"
        "0.....................00"
        "0.....................00"
        "0.....................00"
        "0................F....00"
        "0.............0^^^000000"
        "0........^^^..0...0....."
        "0...@.........0...0....."
        "0000^^^................."
        "0......................."
        "0......................."
        "0.............F........."
        "000000^^^000000000000000"
        "000000...000000000000000"
    },
    {
        {24, 0, 24, 14},
        "000000...............000"
        "000000...............000"
        "0000.................000"
        "000..................000"
        "00...................000"
        "0.....................00"
        ".......................0"
        ".......................0"
        ".......................0"
        "....................F..0"
        "...............F..000000"
        "..............0000000000"
        "0000000000....0000000000"
        "0000000000....0000000000"
    },
    {
        {0, 14, 56, 14},
        "000000...0000000000000000000000000....000000000000000000"
        "000000...0000000000000000000000.........0000000........0"
        "0....0....................................000..........0"
        "0....0....................................000..........0"
        "0....0^^^^0000............................000..........0"
        "0..........00.......................................^^^0"
        "0..........00.....00...................................0"
        "0..........00^^^^^00.............................F.....0"
        "000..F..................000000..................^^^^...0"
        "0000000 ................000000.........................0"
        "0000000 ....@...........000000.........................0"
        "00000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000"
    },
    {}
};


Room const* g_current_room;
Rect        g_room_boundary;


void update_camera() {
    int x = hero->rect().center_x() - camera.w / 2;
    int y = hero->rect().center_y() - camera.h / 2;
    camera.x = clamp(camera.x, x - 40, x + 40);
    camera.y = clamp(camera.y, y - 30, y + 30);
    camera.x = clamp(camera.x, g_room_boundary.x, g_room_boundary.right() - camera.w);
    camera.y = clamp(camera.y, g_room_boundary.y, g_room_boundary.bottom() - camera.h);
}


void load_room(Room const* room) {
    g_current_room = room;

    g_room_boundary = {
        room->rect.x * TILE_SIZE,
        room->rect.y * TILE_SIZE,
        room->rect.w * TILE_SIZE,
        room->rect.h * TILE_SIZE - TILE_SIZE / 2,
    };

    bool init_hero = !hero;
    if (init_hero) {
        hero = new Hero;
        actors.append(hero);
    }

    for (int r = 0; r < room->rect.h; ++r)
    for (int c = 0; c < room->rect.w; ++c) {
        char t = room->tile(c, r);
        if (t == '.') continue;
        int sx = (room->rect.x + c) * TILE_SIZE;
        int sy = (room->rect.y + r) * TILE_SIZE;

        int ax = sx + TILE_SIZE / 2;
        int ay = sy + TILE_SIZE;

        if      (t == '0') solids.append(new Solid(sx, sy));
        else if (t == '^') solids.append(new Solid(sx, sy, true));
        else if (t == 'F') actors.append(new Foe(ax, ay));
        else if (t == '@' && init_hero) {
            hero->init(ax, ay);
            camera.x = hero->rect().center_x() - camera.w / 2;
            camera.y = hero->rect().center_y() - camera.h / 2;
        }
    }
}


template<class T, class Func>
void delete_if(Vector<T*>& vec, Func const& f) {
    int j = 0;
    for (int i = 0; i < vec.len(); ++i) {
        if (f(i)) delete vec[i];
        else vec[j++] = vec[i];
    }
    vec.shrink(j);
}



class {
public:
    bool active() const { return value > 0; }
    bool update() {

        if (value == 0) {
            // did we exit the room?
            int x = hero->rect().center_x() / TILE_SIZE;
            int y = (hero->rect().bottom() - 2)  / TILE_SIZE;
            if (g_current_room->rect.contains(x, y)) return false;

            // find new room
            for (Room const* r = g_rooms; r->data; ++r) {
                if (r == g_current_room) continue;
                if (r->rect.contains(x, y)) {
                    // boost hero up
                    if (y < g_current_room->rect.y) hero->boost();

                    value = 1;
                    old_actors = actors.len();
                    old_solids = solids.len();
                    load_room(r);
                    ox = camera.x;
                    oy = camera.y;
                    update_camera();
                    nx = camera.x;
                    ny = camera.y;
                    camera.x = ox;
                    camera.y = oy;
                    break;
                }
            }

            return true;
        }

        value -= 0.02f;
        if (value < 0) {
            value = 0;

            delete_if(actors, [this](int i) {
                return i < old_actors && actors[i]->type() != ActorType::Hero;
            });
            delete_if(solids, [this](int i) {
                return i < old_solids;
            });
        }

        camera.x = mix(nx, ox, smooth(value));
        camera.y = mix(ny, oy, smooth(value));

        return true;
    }

private:
    float value = 0;
    int   old_solids;
    int   old_actors;
    int   ox, oy;
    int   nx, ny;

} g_transition;


} // namespace


Rect           world::camera = {0, 0, app::WIDTH, app::HEIGHT};
Hero*          world::hero;
Vector<Solid*> world::solids;
Vector<Actor*> world::actors;


void world::init() {
    load_room(&g_rooms[0]);
    update_camera();
}


void world::update() {

    if (g_transition.update()) return;


    for (Solid* s : solids) s->update();

    // use index becaue the vector may grow during updates
    for (int i = 0; i < actors.len(); ++i) {
        actors[i]->update();
    }

    // actor collision
    for (int i = 0; i < actors.len() - 1; ++i) {
        Actor* a = actors[i];
        if (!a->alive()) continue;
        for (int j = i + 1; j < actors.len(); ++j) {
            Actor* b = actors[j];
            if (!b->alive()) continue;
            if (!a->rect().overlap(b->rect())) continue;
            if (a->mask() & Mask(b->type())) a->collide(b);
            if (b->mask() & Mask(a->type())) b->collide(b);
            if (!a->alive()) break;
        }
    }
    delete_if(actors, [](int i) { return !actors[i]->alive(); });


    update_camera();
}


void world::draw() {

    for (Solid* s : solids) s->draw();
    for (Actor* a : actors) a->draw();

    if (g_current_room == &g_rooms[0] && !g_transition.active()) {
        app::print(4, 4, "press Z/C to jump and X to shoot");
    }
}
