#include "foe.hpp"
#include "app.hpp"
#include "world.hpp"


Foe::Foe(int x, int y) : Actor(ActorType::Foe) {
    enum {
        WIDTH  = 20,
        HEIGHT = 24,
    };
    m_rect = { x - WIDTH / 2, y - HEIGHT, WIDTH, HEIGHT };
    m_dir  = 1;
}
void Foe::update() {


//    if (app::input.x != 0) m_dir = app::input.x;
//
//    if (!m_airborne) {
//        m_vx = clamp(MAX_VX * app::input.x, m_vx - 1.0f, m_vx + 1.0f);
//    }
//    else {
//        m_vx = clamp(MAX_VX * app::input.x, m_vx - 0.5f, m_vx + 0.5f);
//    }
//
//    m_rx += m_vx;
//    int mx = round_to_int(m_rx);
//    if (mx != 0) {
//        m_rx -= mx;
//        if (move_x(mx)) {
//            m_vx = 0;
//            m_rx = 0;
//        }
//    }
//
//    if (!m_airborne) {
//        if (app::input.jump_just_pressed()) {
//            m_vy = -7.33333333f;
//            m_ry = 0;
//            m_airborne = true;
//        }
//    }
//
//    m_vy += GRAVITY;
//    m_ry += clamp(m_vy, -MAX_VY, MAX_VY);
//    int my = round_to_int(m_ry);
//    if (my != 0) {
//        m_ry -= my;
//        if (move_y(my)) {
//            m_vy = 0;
//
//            m_ry = 0;
//            m_airborne = false;
//        }
//        else {
//            m_airborne = true;
//        }
//    }
//
//    if (app::input.shoot_just_pressed()) {
//        world::actors.append(new FoeBullet(m_rect.center_x() + m_dir * 5,
//                                           m_rect.center_y(),
//                                           m_dir));
//    }

}

void Foe::draw() const {
    app::screen.rect(m_rect.relative(world::camera), Color(200, 0, 0));
}


void FoeBullet::update() {

    if (move_x(m_dir * 6)) {
        m_alive = false;
        for (int i = 0; i < 10; ++i) {
            world::actors.append(new Particle(m_rect.center_x() + m_dir * 4, m_rect.center_y()));
        }
    }

    if (!m_rect.overlap(world::camera)) {
        m_alive = false;
    }
}
void FoeBullet::draw() const {
    app::screen.rect(m_rect.relative(world::camera), Color(200, 0, 0));
}

