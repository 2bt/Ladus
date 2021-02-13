#include "actor.hpp"
#include "hero.hpp"
#include "app.hpp"
#include "world.hpp"


void Actor::update() {
    switch (m_type) {
    case ActorType::Hero:   get<Hero>()->update(); break;
    case ActorType::Bullet:   get<Bullet>()->update(); break;
    case ActorType::Particle: get<Particle>()->update(); break;
    }
}


void Actor::draw() const {
    switch (m_type) {
    case ActorType::Hero: get<Hero>()->draw(); break;
    case ActorType::Bullet:
    case ActorType::Particle:
        app::screen.rect(m_rect.relative(world::camera), color(200, 200, 200));
        break;
    default:
        app::screen.rect(m_rect.relative(world::camera), color(200, 0, 0));
        break;
    }
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

