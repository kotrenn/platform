#include "bullet.h"

Bullet::Bullet(Actor *parent, BulletShape *shape)
	:m_level(parent->get_level()),
	 m_parent(parent),
	 m_shape(shape),
	 m_team(parent->get_team()),
	 m_dead(false)
{
	m_level->add_bullet(this);
}

Bullet::~Bullet()
{
	delete m_shape;
}

void Bullet::update()
{
	list<Actor *> actors;
	m_level->get_collisions(&actors, get_shape3d(), m_team);
	for (auto a : actors)
	{
		a->damage();
		if (!m_dead)
			die(a);
	}

	if (!actors.empty())
		m_level->remove_bullet(this);
}

void Bullet::move(float dt)
{
	m_shape->move(dt);
}

void Bullet::draw() const
{
	m_shape->draw();
}

bool Bullet::collides(Actor *actor) const
{
	Sphere sphere = actor->get_sphere();
	return sphere.collides(get_shape3d());
}

vector3d Bullet::get_center() const
{
	return get_shape3d()->get_center();
}

void Bullet::actor_dying(Actor *actor)
{
	if (m_parent == actor)
		m_parent = NULL;
}

void Bullet::die(Actor *)
{
	m_dead = true;
}

Shape3d *Bullet::get_shape3d() const
{
	return m_shape->get_shape3d();
}
