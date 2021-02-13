#pragma once

#include "util.hpp"


enum class ActorType {
    Player,
    Bullet,
    Particle,
};


class Actor {
public:

    Actor(ActorType t) : m_type(t), m_alive(true) {}

    ActorType   type() const { return m_type; }
    Rect const& rect() const { return m_rect; }
    bool        alive() const { return m_alive; }

    template<class T> T* get() { return (T*) this; }
    template<class T> T const* get() const { return (T const*) this; }

    bool move_x(int d);
    bool move_y(int d);

    void update();
    void draw() const;

protected:
    ActorType m_type;
    Rect      m_rect;
    bool      m_alive;
};


class Player : public Actor {
public:
    enum {
        WIDTH  = 18,
        HEIGHT = 22,
    };

    Player() : Actor(ActorType::Player) {}

    void boost() { m_vy = -5.5; }

    void init(int x, int y);
    void update();

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
