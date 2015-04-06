#ifndef __CAMERA_H
#define __CAMERA_H

#include "common.h"
#include "surface.h"
#include "actor.h"

class Actor;

class Camera
{
public:
	Camera(const vector3d &pos,
	       const vector2d &size,
	       const vector3d &view_x,
	       const vector3d &view_y);
	~Camera();

	void move(float dt);
	void clear();
	void draw() const;

	float get_x() const { return m_pos[0]; }
	float get_y() const { return m_pos[1]; }
	float get_z() const { return m_pos[2]; }
	int get_w() const { return m_size[0]; }
	int get_h() const { return m_size[1]; }
	int get_d() const { return m_size[2]; }
	const vector3d get_pos() const { return m_pos; }
	const vector3d get_eye() const;
	const vector2d get_size() const { return m_size; }
	const vector3d get_view_x() const { return m_view_x; }
	const vector3d get_view_y() const { return m_view_y; }
	//const Box get_dims() const;
	void set_x(float x) { m_pos[0] = x; }
	void set_y(float y) { m_pos[1] = y; }
	void set_z(float z) { m_pos[2] = z; }
	void set_w(float w) { m_size[0] = w; }
	void set_h(float h) { m_size[1] = h; }
	void set_pos(const vector2d &pos);
	void set_absolute(const vector2d &pos);
	void set_target(Actor *target) { m_target = target; }
	void rotate_space(int val);
	void rotate_gravity(int val);
	bool is_moving() const { return m_moving; }
private:
	vector3d m_pos;
	vector2d m_size;
	vector3d m_view_x;
	vector3d m_view_y;
	Actor *m_target;
	bool m_moving;
	float m_theta_y;
	float m_theta_z;
};

#endif
