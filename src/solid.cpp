#include "app.hpp"
#include "world.hpp"
#include "surfaces.hpp"
#include "actor.hpp"
#include "hero.hpp"


Solid::Solid(int x, int y, bool jumpthrough)
    : m_type(SolidType::Normal)
    , m_rect{x, y, TILE_SIZE, TILE_SIZE}
    , m_jumpthrough(jumpthrough) {}


Solid::Solid(SolidType t, int x, int y, bool jumpthrough)
    : m_type(t)
    , m_rect{x, y, TILE_SIZE, TILE_SIZE}
    , m_jumpthrough(jumpthrough) {}


void Solid::update() {
    switch (m_type) {
    case SolidType::Crate: get<Crate>()->update(); break;
    default: break;
    }
}

void Solid::collide(Actor* a) {
    switch (m_type) {
    case SolidType::Crate: get<Crate>()->collide(a); break;
    default: break;
    }
}

void Solid::draw() const {
    switch (m_type) {
    case SolidType::Crate: get<Crate>()->draw(); return;
    default: break;
    }

    if (!m_collidable) return;
    int t = 0;
    if (m_jumpthrough) t = 1;

    app::screen.copy(surfaces::tile, {t * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE},
                     m_rect.x - world::camera.x,
                     m_rect.y - world::camera.y);
}

void Solid::move(int x, int y) {
}




void Crate::update() {
}
void Crate::collide(Actor* a) {
    if (a->type() == ActorType::Bullet) {
        --m_shield;
        if (m_shield <= 0) {
            m_collidable = false;
            for (int i = 0; i < 16; ++i) {
                Particle* p = new Particle(m_rect.x + 4 + i % 4 * 8,
                                           m_rect.y + 4 + i / 4 * 8,
                                           8, Color(133 / 2, 76 / 2, 48 / 2));
                p->m_ttl = rand_int(10, 30);

                float ang   = rand_float(0, 2 * M_PI);
                float speed = rand_float(3, 5);
                p->m_vx = my_sin(ang) * speed;
                p->m_vy = my_cos(ang) * speed - 2;


                world::actors.append(p);
            }
        }
    }
}
void Crate::draw() const {
    if (m_shield <= 0) return;
    app::screen.copy(surfaces::tile, {TILE_SIZE * 2, 0, TILE_SIZE * 2, TILE_SIZE * 2},
                     m_rect.x - world::camera.x,
                     m_rect.y - world::camera.y);
}
