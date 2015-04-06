#include "bulletshape.h"

BulletShape::BulletShape(Shape3d *shape3d,
                         const vector3d &v)
	:m_shape3d(shape3d),
	 m_v(v)
{
}

BulletShape::~BulletShape()
{
	delete m_shape3d;
}

void BulletShape::move(float dt)
{
	*m_shape3d += dt * m_v;
}



BoxBullet::BoxBullet(const Box &box,
                     const vector3d &v)
	:BulletShape(new Box(box), v)
{
}

BoxBullet::~BoxBullet()
{
}

void BoxBullet::draw() const
{
	m_shape3d->draw();
}



SphereBullet::SphereBullet(const Sphere &sphere,
                           const vector3d &v)
	:BulletShape(new Sphere(sphere), v)
{
}

SphereBullet::~SphereBullet()
{
}

void SphereBullet::draw() const
{
	m_shape3d->draw();
}
