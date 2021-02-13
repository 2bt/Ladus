#pragma once

#include "actor.hpp"


class Hero : public Actor {
public:
    Hero() : Actor(ActorType::Hero) {}
    void init(int x, int y);
    void update();
    void draw() const;

    void boost() { m_vy = -5.5; }

private:
    float m_rx;
    float m_ry;
    float m_vx;
    float m_vy;
    bool  m_airborne;
    int   m_dir;
};


class Bullet : public Actor {
public:
    Bullet(int x, int y, int dir) : Actor(ActorType::Bullet) {
        m_rect = { x - 5, y - 1, 10, 2 };
        m_dir  = dir;
    }
    void update();
private:
    int m_dir;
};


class Particle : public Actor {
public:
    Particle(int x, int y);
    void update();
private:
    float m_rx = 0;
    float m_ry = 0;
    float m_vx;
    float m_vy;
    int   m_ttl;
};

