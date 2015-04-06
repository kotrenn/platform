#ifndef __BULLET_SHAPE_H
#define __BULLET_SHAPE_H

#include "common.h"
#include "camera.h"

class Camera;

class BulletShape
{
public:
	BulletShape(Shape3d *shape3d,
	            const vector3d &v);
	virtual ~BulletShape();

	void move(float dt);
	virtual void draw() const = 0;

	Shape3d *get_shape3d() const { return m_shape3d; }
	vector3d &get_v() { return m_v; }
	const vector3d &get_v() const { return m_v; }
	void set_v(const vector3d &v) { m_v = v; }
protected:
	Shape3d *m_shape3d;
	vector3d m_v;
};

class BoxBullet : public BulletShape
{
public:
	BoxBullet(const Box &box,
	           const vector3d &v);
	~BoxBullet();

	void draw() const;
};

class SphereBullet : public BulletShape
{
public:
	SphereBullet(const Sphere &sphere,
	             const vector3d &v);
	~SphereBullet();
	
	void draw() const;
};

#endif
