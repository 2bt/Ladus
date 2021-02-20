#pragma once

#include "util.hpp"


#define ACTORS(X) \
    X(Hero)       \
    X(Bullet)     \
    X(Particle)   \
    X(Foe)        \
    X(FoeBullet)


#define CREATE_ENUM(NAME) NAME,
enum class ActorType { ACTORS(CREATE_ENUM) };

inline uint32_t Mask() { return 0; }
template<class... Args>
uint32_t Mask(ActorType t, Args... args) {
    return (1 << int(t)) | Mask(args...);
}


class Solid;

class Actor {
public:

    Actor(ActorType t, uint32_t mask = 0) : m_type(t), m_mask(mask), m_alive(true) {}

    ActorType   type() const { return m_type; }
    Rect const& rect() const { return m_rect; }
    uint32_t    mask() const { return m_mask; }
    bool        alive() const { return m_alive; }

    template<class T> T* get() { return (T*) this; }
    template<class T> T const* get() const { return (T const*) this; }

    Solid* move_x(int d);
    Solid* move_y(int d);

    void update();
    void collide(Actor* a);
    void draw() const;

protected:
    ActorType m_type;
    uint32_t  m_mask;
    bool      m_alive;
    Rect      m_rect;
};

