#include "actor.hpp"
#include "hero.hpp"
#include "foe.hpp"
#include "app.hpp"
#include "world.hpp"


#define DISPATCH(T, F) case ActorType::T: get<T>()->F; break;
#define DISTATCH_UPDATE(T)  DISPATCH(T, update());
#define DISTATCH_DRAW(T)    DISPATCH(T, draw());
#define DISTATCH_COLLIDE(T) DISPATCH(T, collide(a));


void Actor::update() {
    switch (m_type) { ACTORS(DISTATCH_UPDATE); }
}

void Actor::draw() const {
    switch (m_type) { ACTORS(DISTATCH_DRAW); }
}

void Actor::collide(Actor* a) {
    switch (m_type) { ACTORS(DISTATCH_COLLIDE); }
}


Solid* Actor::move_x(int d) {
    m_rect.x += d;
    int    o  = 0;
    Solid* cs = nullptr;
    for (Solid* s : world::solids) {
        if (!s->collidable()) continue;
        if (s->jumpthrough()) continue;
        int oo = s->rect().overlap_x(m_rect);
        if (abs(oo) > abs(o)) {
            o = oo;
            cs = s;
        }
    }
    if (o == 0) return nullptr;
    m_rect.x -= o;
    return cs;
}

Solid* Actor::move_y(int d) {
    int bottom = m_rect.y + m_rect.h;
    m_rect.y += d;
    int    o  = 0;
    Solid* cs = nullptr;
    for (Solid* s : world::solids) {
        if (!s->collidable()) continue;
        if (s->jumpthrough() && bottom > s->rect().y) continue;
        int oo = s->rect().overlap_y(m_rect);
        if (abs(oo) > abs(o)) {
            o = oo;
            cs = s;
        }
    }
    if (o == 0) return nullptr;
    m_rect.y -= o;
    return cs;
}

