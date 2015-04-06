#ifndef __SHAPE_3D_H
#define __SHAPE_3D_H

#include "vector3d.h"

class vector3d;

class Sphere;
class Box;

class Shape3d
{
public:
	virtual ~Shape3d();
	virtual bool collides(const Shape3d *shape3d) const = 0;
	virtual bool visit_collides(const Sphere *sphere) const = 0;
	virtual bool visit_collides(const Box *box) const = 0;
	virtual vector3d get_center() const = 0;
	virtual const Shape3d &operator+=(const vector3d &rhs) = 0;
	void fix_box(const Box &box, vector3d *v = NULL);
	virtual void fix_box_x(const Box &box, vector3d *v) = 0;
	virtual void fix_box_y(const Box &box, vector3d *v) = 0;
	virtual void fix_box_z(const Box &box, vector3d *v) = 0;
	virtual void print() const = 0;
	virtual void draw() const = 0;
};

class Sphere : public Shape3d
{
public:
	Sphere();
	Sphere(float x, float y, float z, float r);
	Sphere(const vector3d &c, float r);

	const float &operator[](int i) const;
	float &operator[](int i);

	bool collides(const Shape3d *shape3d) const;
	bool visit_collides(const Sphere *sphere) const;
	bool visit_collides(const Box *box) const;
	vector3d get_center() const;
	void set_center(const vector3d &center);
	const Shape3d &operator+=(const vector3d &rhs);
	void fix_box_x(const Box &box, vector3d *v = NULL);
	void fix_box_y(const Box &box, vector3d *v = NULL);
	void fix_box_z(const Box &box, vector3d *v = NULL);
	void print() const;
	void draw() const;

	float get_x() const { return m_x; }
	float get_y() const { return m_y; }
	float get_z() const { return m_z; }
	float get_r() const { return m_r; }
	void set_x(float x) { m_x = x; }
	void set_y(float y) { m_y = y; }
	void set_z(float z) { m_z = z; }
	void set_r(float r) { m_r = r; }
private:
	static bool m_list_made;
	static int m_display_list;
	static void make_list();
	
	float m_x, m_y, m_z;
	float m_r;
};

class Box : public Shape3d
{
public:
	Box();
	Box(float x, float y, float z,
	    float w, float h, float d);
	Box(const vector3d &pos,
	    const vector3d &size);

	const float &operator[](int i) const;
	float &operator[](int i);

	bool collides(const Shape3d *shape3d) const;
	bool visit_collides(const Sphere *sphere) const;
	bool visit_collides(const Box *box) const;
	vector3d get_center() const;
	const Shape3d &operator+=(const vector3d &rhs);
	void fix_box_x(const Box &box, vector3d *v = NULL);
	void fix_box_y(const Box &box, vector3d *v = NULL);
	void fix_box_z(const Box &box, vector3d *v = NULL);
	void print() const;
	void draw() const;
	
	bool contains(float x, float y, float z) const;

	float get_x() const { return m_x; }
	float get_y() const { return m_y; }
	float get_z() const { return m_z; }
	float get_w() const { return m_w; }
	float get_h() const { return m_h; }
	float get_d() const { return m_d; }
	void set_x(float x) { m_x = x; }
	void set_y(float y) { m_y = y; }
	void set_z(float z) { m_z = z; }
	void set_w(float w) { m_w = w; }
	void set_h(float h) { m_h = h; }
	void set_d(float d) { m_d = d; }
private:
	static bool m_list_made;
	static int m_display_list;
	static void make_list();
	
	float m_x, m_y, m_z;
	float m_w, m_h, m_d;
};

#endif
