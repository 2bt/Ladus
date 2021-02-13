#pragma once

#include "actor.hpp"


class Foe : public Actor {
public:
    Foe(int x, int y);
    void update();
    void collide(Actor* a) {}
    void draw() const;

private:
    float m_rx;
    float m_ry;
    float m_vx;
    float m_vy;
    bool  m_airborne;
    int   m_dir;
};


class FoeBullet : public Actor {
public:
    FoeBullet(int x, int y, int dir) : Actor(ActorType::FoeBullet) {
        m_rect = { x - 5, y - 1, 10, 2 };
        m_dir  = dir;
    }
    void update();
    void collide(Actor* a) {}
    void draw() const;
private:
    int m_dir;
};
