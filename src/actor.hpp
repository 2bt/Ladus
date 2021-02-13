#pragma once

#include "util.hpp"


enum class ActorType {
    Hero,
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

