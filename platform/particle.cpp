#include "particle.h"

Particle::Particle(Level *level, Uint32 color,
                   const Sphere &pos,
                   const vector3d &v,
                   float time)
	:m_level(level),
	 m_color(color),
	 m_pos(pos),
	 m_v(v),
	 m_timer(time)
{
	m_level->add_particle(this);
}

Particle::~Particle()
{
}

void Particle::pause()
{
	m_timer.pause();
}

void Particle::unpause()
{
	m_timer.unpause();
}

void Particle::update()
{
}

void Particle::move(float dt)
{
	m_pos += m_v * dt;
}

void Particle::draw() const
{
	m_pos.draw();
	/*
	Surface *dst = camera->get_surface();
	int x = (int)(m_pos[0] - camera->get_x());
	int y = (int)(m_pos[1] - camera->get_y());
	int r = (int)(m_pos[2]);
	if (x + r < 0 ||
	    y + r < 0 ||
	    x - r > camera->get_w() ||
	    y - r > camera->get_h())
		return;
	dst->draw_circle(m_color, x, y, r);
	*/
}

bool Particle::dead() const
{
	return m_timer.done();
}
