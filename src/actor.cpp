#include "actor.hpp"
#include "app.hpp"
#include "world.hpp"


void Actor::update() {
    if (m_type == ActorType::Player) get<Player>()->update();
    if (m_type == ActorType::Bullet) get<Bullet>()->update();
    if (m_type == ActorType::Particle) get<Particle>()->update();
}


void Actor::draw() const {
    app::screen.rect(m_rect, color(200, 200, 0));
}

bool Actor::move_x(int d) {
    m_rect.x += d;
    int o = 0;
    for (Solid* s : world::solids) {
        if (s->jumpthrough()) continue;
        int oo = s->rect().overlap_x(m_rect);
        if (abs(oo) > abs(o)) o = oo;
    }
    if (o == 0) return false;
    m_rect.x -= o;
    return true;
}

bool Actor::move_y(int d) {
    int bottom = m_rect.y + m_rect.h;
    m_rect.y += d;
    int o = 0;
    for (Solid* s : world::solids) {
        if (s->jumpthrough() && bottom > s->rect().y) continue;
        int oo = s->rect().overlap_y(m_rect);
        if (abs(oo) > abs(o)) o = oo;
    }
    if (o == 0) return false;
    m_rect.y -= o;
    return true;
}



void Player::init(int x, int y) {
    m_rect = {
        x - WIDTH / 2,
        y - HEIGHT,
        WIDTH,
        HEIGHT,
    };

    m_rx       = 0;
    m_ry       = 0;
    m_vx       = 0;
    m_vy       = 0;
    m_airborne = true;
    m_dir      = 1;
}

constexpr float GRAVITY = 0.5f;
constexpr float MAX_VX  = 2.0f;
constexpr float MAX_VY  = 4.5f;


void Player::update() {


    if (app::input.x != 0) m_dir = app::input.x;

    if (!m_airborne) {
        m_vx = clamp(MAX_VX * app::input.x, m_vx - 1.0f, m_vx + 1.0f);
    }
    else {
        m_vx = clamp(MAX_VX * app::input.x, m_vx - 0.5f, m_vx + 0.5f);
    }

    m_rx += m_vx;
    int mx = int(m_rx);
    if (mx != 0) {
        m_rx -= mx;
        if (move_x(mx)) {
            m_vx = 0;
            m_rx = 0;
        }
    }

    if (!m_airborne) {
        if (app::input.jump_just_pressed()) {
            m_vy = -7;
            m_airborne = true;
        }
    }

    m_vy += GRAVITY;
    m_ry += clamp(m_vy, -MAX_VY, MAX_VY);
    int my = int(m_ry);
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

    if (app::input.shoot_just_pressed()) {
        world::actors.append(new Bullet(m_rect.center_x() + m_dir * 5,
                                        m_rect.center_y(),
                                        m_dir));
    }
}


void Bullet::update() {

    if (move_x(m_dir * 6)) {
        m_alive = false;
        for (int i = 0; i < 10; ++i) {
            world::actors.append(new Particle(m_rect.center_x() + m_dir * 4, m_rect.center_y()));
        }
    }

    if (!m_rect.overlap({0, 0, app::WIDTH, app::HEIGHT})) {
        m_alive = false;
    }
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
    int mx = int(m_rx);
    m_rx -= mx;
    if (move_x(mx)) {
        m_vx *= -0.9;
        m_rx = 0;
    }

    m_vy += GRAVITY;
    m_ry += clamp(m_vy, -MAX_VY, MAX_VY);
    int my = int(m_ry);
    if (my != 0) {
        m_ry -= my;
        if (move_y(my)) {
            m_vy *= -0.9;
            m_ry = 0;
        }
    }
}
