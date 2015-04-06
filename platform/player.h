#ifndef __PLAYER_H
#define __PLAYER_H

#include "actor.h"
#include "input.h"

class PlayerActor : public Actor, InputListener
{
public:
	PlayerActor(Level *level, InputDevice *device);
	virtual ~PlayerActor();

	void pause();
	void unpause();

	void damage();

	void reset();
	void update();
	void draw() const;
	virtual void draw_interface(Surface *) const {}

	void button_pressed(Button button);
protected:
	void create_bullet();
	
	float m_fire_rate;
	Timer m_fire_timer;
	Timer m_select_timer;
	Timer m_jump_timer;
	Timer m_health_timer;
};

#endif
