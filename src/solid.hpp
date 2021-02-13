#pragma once

#include "util.hpp"


//enum class SolidType {
//    Normal,
//};


class Solid {
public:

    Solid(int x, int y, bool jumpthrough = false);

//    SolidType   type() const { return m_type; }
    Rect const& rect() const { return m_rect; }
    bool        jumpthrough() const { return m_jumpthrough; }

//    template<class T> T* get() { return (T*) this; }
//    template<class T> T const* get() const { return (T const*) this; }

    void update();
    void draw() const;

protected:
    void move(int x, int y);

//    SolidType m_type;
    Rect      m_rect;
    bool      m_jumpthrough;
};
