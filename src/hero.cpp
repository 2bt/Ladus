#include "hero.hpp"
#include "app.hpp"
#include "world.hpp"
#include "foe.hpp"


constexpr float GRAVITY = 0.333333333f;
constexpr float MAX_VX  = 2.0f;
constexpr float MAX_VY  = 4.5f;


void Hero::init(int x, int y) {
    enum {
        WIDTH  = 16,
        HEIGHT = 20,
    };

    m_rect = { x - WIDTH / 2, y - HEIGHT, WIDTH, HEIGHT };

    m_rx       = 0;
    m_ry       = 0;
    m_vx       = 0;
    m_vy       = 0;
    m_airborne = true;
    m_dir      = 1;
    m_shoot_delay = 0;
}


void Hero::update() {


    if (app::input.x != 0) m_dir = app::input.x;

    if (!m_airborne) {
        m_vx = clamp(MAX_VX * app::input.x, m_vx - 1.0f, m_vx + 1.0f);
    }
    else {
        m_vx = clamp(MAX_VX * app::input.x, m_vx - 0.5f, m_vx + 0.5f);
    }

    m_rx += m_vx;
    int mx = round_to_int(m_rx);
    if (mx != 0) {
        m_rx -= mx;
        if (move_x(mx)) {
            m_vx = 0;
            m_rx = 0;
        }
    }

    if (!m_airborne) {
        if (app::input.jump_just_pressed()) {
            // jump
            m_vy = -7.33333333f;
            m_ry = 0;
            m_airborne = true;
        }
    }
    else {
        if (app::input.jump_just_released()) {
            // limit jump hight
            m_vy = max(m_vy, -1.5f);
        }
    }

    m_vy += GRAVITY;
    m_ry += clamp(m_vy, -MAX_VY, MAX_VY);
    int my = round_to_int(m_ry);
    if (my != 0) {
        m_ry -= my;
        if (move_y(my)) {
            m_vy = 0;
            m_ry = 0;
            m_airborne = false;
        }
        else {
            m_airborne = true;
        }
    }

    if (m_shoot_delay > 0) --m_shoot_delay;
    else if (app::input.shoot) {
        m_shoot_delay = 12;
        world::actors.append(new Bullet(m_rect.center_x() + m_dir * 5,
                                        m_rect.center_y(),
                                        m_dir));
    }
}
void Hero::draw() const {
    app::screen.rect(m_rect.relative(world::camera), Color(200, 200, 0));
}


void Bullet::update() {

    if (move_x(m_dir * 6)) {
        m_alive = false;
        for (int i = 0; i < 10; ++i) {
            world::actors.append(new Particle(m_rect.center_x() + m_dir * 4, m_rect.center_y()));
        }
        return;
    }

    if (!m_rect.overlap(world::camera)) {
        m_alive = false;
    }
}
void Bullet::collide(Actor* a) {
    m_alive = false;
    for (int i = 0; i < 10; ++i) {
        world::actors.append(new Particle(m_rect.center_x() + m_dir * 4, m_rect.center_y()));
    }

}
void Bullet::draw() const {
    app::screen.rect(m_rect.relative(world::camera), Color(200, 200, 200));
}


Particle::Particle(int x, int y) : Actor(ActorType::Particle) {
    m_rect = { x - 1, y - 1, 2, 2 };
    m_vx   = rand_float(-4, 2);
    m_vy   = rand_float(-4, 1);
    m_ttl  = rand_int(1, 8);
}
void Particle::update() {

    m_alive = m_ttl-- > 0;

    m_rx += m_vx;
    int mx = round_to_int(m_rx);
    m_rx -= mx;
    if (move_x(mx)) {
        m_vx *= -0.9;
        m_rx = 0;
    }

    m_vy += GRAVITY;
    m_ry += clamp(m_vy, -MAX_VY, MAX_VY);
    int my = round_to_int(m_ry);
    if (my != 0) {
        m_ry -= my;
        if (move_y(my)) {
            m_vy *= -0.9;
            m_ry = 0;
        }
    }
}
void Particle::draw() const {
    app::screen.rect(m_rect.relative(world::camera), Color(200, 200, 200));
}

