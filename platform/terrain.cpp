#include "terrain.h"

Terrain::Terrain()
	:m_type(NULL),
	 m_dims()
{
}

Terrain::Terrain(const Box &dims,
                 TerrainType *type)
	:m_type(type),
	 m_dims(dims)
{
}

Terrain::~Terrain()
{
}

void Terrain::fix(Actor *actor, Camera *camera) const
{
	if (m_type->is_invisible())
		return;
	
	vector3d v = actor->get_v();
	Sphere sphere = actor->get_sphere();

	//if (sphere.collides(&m_dims))
	if (m_dims.collides(&sphere))
	{
		float sx = sphere.get_x();
		float sy = sphere.get_y();
		float sz = sphere.get_z();
		
		float cx, cy, cz;
		
		if (sx < m_dims.get_x())
			cx = m_dims.get_x();
		else if (sx > m_dims.get_x() + m_dims.get_w())
			cx = m_dims.get_x() + m_dims.get_w();
		else
			cx = sx;

		if (sy < m_dims.get_y())
			cy = m_dims.get_y();
		else if (sy > m_dims.get_y() + m_dims.get_h())
			cy = m_dims.get_y() + m_dims.get_h();
		else
			cy = sy;

		if (sz < m_dims.get_z())
			cz = m_dims.get_z();
		else if (sz > m_dims.get_z() + m_dims.get_d())
			cz = m_dims.get_z() + m_dims.get_d();
		else
			cz = sz;

		vector3d hit(cx, cy, cz);

		vector3d center = sphere.get_center();
		float diff = sphere.get_r() - (center - hit).norm();
		vector3d disp = center - hit;
		if (disp.norm() < EPSILON)
			return;
		disp.normalize();
		center += diff * disp;
		actor->set_center(center);

		bool ceiling = false;
		bool floor = false;
		
		vector3d view_y = -1 * camera->get_view_y();
		if (view_y * disp > 0) // ceiling
			ceiling = true;
		if (view_y * disp < -0.55)
			//if (view_y * disp < -0.001)
		    floor = true;

		//ceiling = false;
		//floor = true;

		if (ceiling)
		{
			actor->clear_jump();
			if (view_y * v > 0)
			{
				actor->reset_grav();
				vector3d proj = (v * view_y) * view_y;
				v -= 2.0 * proj;
				actor->set_v(v);
			}
			//printf("ceiling\n");
			//actor->stop_jumping();
			//actor->set_v(vector3d(0, 0, 0));
		}
		//if (floor && !actor->is_jumping())
		if (floor)
		{
			//actor->stop_jumping();
			actor->reset_grav();
			actor->reset_jumpable();
		}
	}

	//plane_fix(actor, axis, );

	/*
	vector2d v = actor->get_v();
	Circle circle = actor->get_circle();

	vector2d p1(m_dims[0], m_y1);
	vector2d p2(m_dims[0] + m_dims[2], m_y2);
	vector2d p3(m_dims[0] + m_dims[2], m_y4);
	vector2d p4(m_dims[0], m_y3);

	float slope = (p2[1] - p1[1]) / (p2[0] - p1[0]);
	bool jump = fabs(slope) < 1.5;
	line_fix(actor, p1, p2, jump, false);
	line_fix(actor, p2, p3, false, false);
	line_fix(actor, p3, p4, false, true);
	line_fix(actor, p4, p1, false, false);
	*/
}

void Terrain::line_fix(Actor *actor,
                       const vector2d &p1,
                       const vector2d &p2,
                       bool jump, bool bottom) const
{
	return;

	/*
	vector2d v = actor->get_v();
	Circle circle = actor->get_circle();

	Line line(p1[0], p1[1], p2[0], p2[1]);
	if (line.collides(&circle))
	{
		v.set_y(0);
		vector2d u = p2 - p1;
		if (u.norm() < EPSILON)
			return;
		u.normalize();
		vector2d c(circle[0], circle[1]);
		vector2d p1c = c - p1;
		vector2d x = p1 + (p1c * u) * u;
		if ((x - p1) * u < 0)
			return;
		if ((p2 - x) * u < 0)
			return;
		vector2d xc = c - x;
		float r = circle[2];
		float n = xc.norm();
		if (fabs(n) < EPSILON)
		{
			xc = vector2d(u[1], -u[0]);
			n = xc.norm();
		}
		vector2d cp = c + (r - n) * (xc / n);
		if (bottom || jump)
			actor->set_v(v);
		actor->set_center(cp);
		if (bottom)
			actor->set_falling(true);
		if (jump)
		{
			actor->set_jump(-1);
			actor->set_falling(false);
		}
	}
	*/
}

bool Terrain::collides(const Bullet *bullet) const
{
	if (m_type->is_invisible())
		return false;
	
	Shape3d *shape3d = bullet->get_shape3d();
	return m_dims.collides(shape3d);
	/*
	vector2d p1(m_dims[0], m_y1);
	vector2d p2(m_dims[0] + m_dims[2], m_y2);
	vector2d p3(m_dims[0] + m_dims[2], m_y4);
	vector2d p4(m_dims[0], m_y3);

	Line line1(p1, p2);
	Line line2(p2, p3);
	Line line3(p3, p4);
	Line line4(p4, p1);

	Shape *shape = bullet->get_shape();

	if (line1.collides(shape) && line1.len() > EPSILON)
		return true;
	if (line2.collides(shape) && line2.len() > EPSILON)
		return true;
	if (line3.collides(shape) && line3.len() > EPSILON)
		return true;
	if (line4.collides(shape) && line4.len() > EPSILON)
		return true;

	// this could be any point in the interior of shape
	vector2d p = shape->get_center();

	vector2d v1 = p2 - p1;
	vector2d v2 = p3 - p2;
	vector2d v3 = p4 - p3;
	vector2d v4 = p1 - p4;

	vector2d i1 = p - p1;
	vector2d i2 = p - p2;
	vector2d i3 = p - p3;
	vector2d i4 = p - p4;

	if (v1 % i1 < 0 || v2 % i2 < 0 ||
	    v3 % i3 < 0 || v4 % i4 < 0)
		return true;

	return false;
	*/
}

void Terrain::draw() const
{
	if (!m_type->is_invisible())
		m_type->draw(m_dims);
}
