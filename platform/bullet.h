#ifndef __BULLET_H
#define __BULLET_H

#include "actor.h"
#include "bulletshape.h"
#include "level.h"

class Actor;
class BulletShape;
class Level;

class Bullet
{
public:
	Bullet(Actor *parent, BulletShape *shape);
	virtual ~Bullet();

	virtual void pause() {}
	virtual void unpause() {}

	virtual void update();
	virtual void move(float dt);
	virtual void draw() const;

	int get_team() const { return m_team; }
	bool collides(Actor *actor) const;
	vector3d get_center() const;
	virtual void actor_dying(Actor *actor);
	bool dead() const { return m_dead; }
	virtual void die(Actor *source);

	Shape3d *get_shape3d() const;
protected:
	Level *m_level;
	Actor *m_parent;
	BulletShape *m_shape;
	int m_team;
	bool m_dead;
};

#endif
