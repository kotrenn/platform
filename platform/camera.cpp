#include "camera.h"

Camera::Camera(const vector3d &pos,
               const vector2d &size,
               const vector3d &view_x,
               const vector3d &view_y)
	:m_pos(pos),
	 m_size(size),
	 m_view_x(view_x),
	 m_view_y(view_y),
	 m_target(NULL),
	 m_moving(false),
	 m_theta_y(0),
	 m_theta_z(0)
{
	printf("camera::view_x = %f %f %f\n", view_x[0], view_x[1], view_x[2]);
	printf("camera::view_y = %f %f %f\n", view_y[0], view_y[1], view_y[2]);
	fflush(stdout);
}

Camera::~Camera()
{
	printf("Camera::~Camera()\n");
	fflush(stdout);
}

void Camera::move(float dt)
{
	if (m_target != NULL)
	{
		vector3d center = m_target->get_center();
		m_pos = center;
		//m_pos -= m_size[0] * m_view_x;
		//m_pos -= m_size[1] * m_view_y;
	}

	if (m_moving)
	{
		float omega = 120.0; // angular velocity
		
		if (fabs(m_theta_y) > 0)
		{
			float theta_y = m_theta_y;
			theta_y -= omega * dt * signum(m_theta_y);
			if (signum(theta_y) != signum(m_theta_y) ||
			    fabs(theta_y) < EPSILON)
			{
				m_moving = false;
				m_theta_y = 0;
			}
			else
				m_theta_y = theta_y;
		}
		
		if (fabs(m_theta_z) > 0)
		{
			float theta_z = m_theta_z;
			theta_z -= omega * dt * signum(m_theta_z);
			if (signum(theta_z) != signum(m_theta_z) ||
			    fabs(theta_z) < EPSILON)
			{
				m_moving = false;
				m_theta_z = 0;
			}
			else
				m_theta_z = theta_z;
		}
	}
}

void Camera::draw() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float theta = 60.0;
	float aspect = m_size[0] / m_size[1];
	float n = 20;
	float f = 100;
	gluPerspective(theta, aspect, n / 2, f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	vector3d pos = m_pos;
	vector3d z = m_view_x % m_view_y;
	vector3d loc = z * n;
	if (fabs(m_theta_y) > 0)
		loc = m_view_y.rotate(-m_theta_y, loc);
	loc += pos;
	vector3d up = m_view_y;
	if (fabs(m_theta_z) > 0)
		up = z.rotate(-m_theta_z, up);
	gluLookAt(loc[0], loc[1], loc[2],
	          pos[0], pos[1], pos[2],
	          up[0], up[1], up[2]);
	
	return;

	// stuff for drawing from a fixed angle

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 30);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float view = 20;
	gluLookAt(view, view, view,
	          0, 0, 0,
	          0, 0, 1);
	*/
}

const vector3d Camera::get_eye() const
{
	float n = 20;
	vector3d pos = m_pos;
	vector3d z = m_view_x % m_view_y;
	vector3d loc = z * n;
	if (fabs(m_theta_y) > 0)
		loc = m_view_y.rotate(-m_theta_y, loc);
	loc += pos;
	return loc;
}

/*
  const Box Camera::get_dims() const
  {
  Box ret(m_x, m_y, m_z, m_w, m_h, INFINITY);
  return ret;
  }
*/

void Camera::set_pos(const vector2d &pos)
{
	m_pos = pos;
}

void Camera::set_absolute(const vector2d &pos)
{
	//m_pos = m_target_pos = pos;
	m_pos = pos;
}

void Camera::rotate_space(int val)
{
	m_view_x = m_view_y.rotate(90 * val, m_view_x);

	m_moving = true;
	m_theta_y = val * 90;

	printf("m_view_x = [%f %f %f]\n", m_view_x[0], m_view_x[1], m_view_x[2]);
	printf("m_view_y = [%f %f %f]\n", m_view_y[0], m_view_y[1], m_view_y[2]);
}

void Camera::rotate_gravity(int val)
{
	vector3d view_z = m_view_x % m_view_y;
	m_view_x = view_z.rotate(90 * val, m_view_x);
	m_view_y = view_z.rotate(90 * val, m_view_y);

	m_moving = true;
	m_theta_z = val * 90;

	printf("m_view_x = [%f %f %f]\n", m_view_x[0], m_view_x[1], m_view_x[2]);
	printf("m_view_y = [%f %f %f]\n", m_view_y[0], m_view_y[1], m_view_y[2]);
}

