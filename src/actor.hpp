#pragma once

#include "surface.hpp"


enum class ActorType {
    Player
};


class Actor {
public:

    ActorType   type() const { return m_type; }
    Rect const& rect() const { return m_rect; }
    template<class T> T* get() { return (T*) this; }
    template<class T> T const* get() const { return (T const*) this; }

    bool move_x(int d);
    bool move_y(int d);

    void update();
    void draw() const;

protected:
    ActorType m_type;
    Rect      m_rect;
};


class Player : public Actor {
public:
    enum {
        WIDTH  = 20,
        HEIGHT = 24,
    };

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
