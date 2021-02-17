#pragma once

#include "util.hpp"

class Actor;

enum class SolidType {
    Normal,
    Crate,
};

class Solid {
public:

    Solid(int x, int y, bool jumpthrough = false);
    Solid(SolidType t, int x, int y, bool jumpthrough = false);

    SolidType   type() const { return m_type; }
    Rect const& rect() const { return m_rect; }
    bool        jumpthrough() const { return m_jumpthrough; }
    bool        collidable() const { return m_collidable; }

    template<class T> T* get() { return (T*) this; }
    template<class T> T const* get() const { return (T const*) this; }

    void update();
    void collide(Actor* a);
    void draw() const;

protected:
    void move(int x, int y);

    SolidType m_type;
    Rect      m_rect;
    bool      m_jumpthrough;
    bool      m_collidable = true;
};


class Crate : public Solid {
public:
    Crate(int x, int y) : Solid(SolidType::Crate, x, y) {
        m_rect.w *= 2;
        m_rect.h *= 2;
    }

    void update();
    void collide(Actor* a);
    void draw() const;

private:
    int m_shield = 5;
};
