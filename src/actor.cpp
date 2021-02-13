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


bool Actor::move_x(int d) {
    m_rect.x += d;
    int o = 0;
    for (Solid* s : world::solids) {
        if (s->jumpthrough()) continue;
        int oo = s->rect().overlap_x(m_rect);
        if (abs(oo) > abs(o)) o = oo;
    }
    if (o == 0) return false;
    m_rect.x -= o;
    return true;
}

bool Actor::move_y(int d) {
    int bottom = m_rect.y + m_rect.h;
    m_rect.y += d;
    int o = 0;
    for (Solid* s : world::solids) {
        if (s->jumpthrough() && bottom > s->rect().y) continue;
        int oo = s->rect().overlap_y(m_rect);
        if (abs(oo) > abs(o)) o = oo;
    }
    if (o == 0) return false;
    m_rect.y -= o;
    return true;
}

