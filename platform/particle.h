#ifndef __PARTICLE_H
#define __PARTICLE_H

#include "level.h"
#include "timer.h"

class Level;

class Particle
{
public:
	Particle(Level *level, Uint32 color,
	         const Sphere &pos,
	         const vector3d &v, float time);
	~Particle();

	void pause();
	void unpause();

	void update();
	void move(float dt);
	void draw() const;

	bool dead() const;
private:
	Level *m_level;
	Uint32 m_color;
	Sphere m_pos;
	vector3d m_v;
	Timer m_timer;
};

#endif
