#include "shape3d.h"

Shape3d::~Shape3d()
{
}

void Shape3d::fix_box(const Box &box, vector3d *v)
{
	fix_box_x(box, v);
	fix_box_y(box, v);
	fix_box_z(box, v);
}



bool Sphere::m_list_made = false;
int Sphere::m_display_list = 0;

Sphere::Sphere()
	:m_x(0), m_y(0), m_z(0),
	 m_r(0)
{
}

Sphere::Sphere(float x, float y, float z, float r)
	:m_x(x), m_y(y), m_z(z),
	 m_r(r)
{
}

Sphere::Sphere(const vector3d &c, float r)
	:m_x(c[0]), m_y(c[1]), m_z(c[2]),
	 m_r(r)
{
}

const float &Sphere::operator[](int i) const
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else if (i == 2)
		return m_z;
	else
		return m_r;
}

float &Sphere::operator[](int i)
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else if (i == 2)
		return m_z;
	else
		return m_r;
}

bool Sphere::collides(const Shape3d *shape3d) const
{
	return shape3d->visit_collides(this);
}

bool Sphere::visit_collides(const Sphere *sphere) const
{
	float dx = m_x - sphere->m_x;
	float dy = m_y - sphere->m_y;
	float dz = m_z - sphere->m_z;
	float r = m_r + sphere->m_r;
	return dx * dx + dy * dy + dz * dz <= r * r;
}

bool Sphere::visit_collides(const Box *box) const
{
	if (m_r < EPSILON ||
	    box->get_w() < EPSILON ||
	    box->get_h() < EPSILON ||
	    box->get_d() < EPSILON)
		return false;
	
	float cx, cy, cz;

	if (m_x < box->get_x())
		cx = box->get_x();
	else if (m_x > box->get_x() + box->get_w())
		cx = box->get_x() + box->get_w();
	else
		cx = m_x;
	
	if (m_y < box->get_y())
		cy = box->get_y();
	else if (m_y > box->get_y() + box->get_h())
		cy = box->get_y() + box->get_h();
	else
		cy = m_y;

	if (m_z < box->get_z())
		cz = box->get_z();
	else if (m_z > box->get_z() + box->get_d())
		cz = box->get_z() + box->get_d();
	else
		cz = m_z;

	float dx = m_x - cx;
	float dy = m_y - cy;
	float dz = m_z - cz;
	return dx * dx + dy * dy + dz * dz <= m_r * m_r;
}

vector3d Sphere::get_center() const
{
	return vector3d(m_x, m_y, m_z);
}

void Sphere::set_center(const vector3d &center)
{
	m_x = center[0];
	m_y = center[1];
	m_z = center[2];
}

const Shape3d &Sphere::operator+=(const vector3d &rhs)
{
	m_x += rhs[0];
	m_y += rhs[1];
	m_z += rhs[2];
	return *this;
}

void Sphere::fix_box_x(const Box &box, vector3d *v)
{
	float vx = 0;
	if (v != NULL)
		vx = v->get_x();

	if (m_x - m_r < box.get_x() && vx <= 0)
	{
		if (v != NULL)
			v->set_x(-1 * v->get_x());
		m_x = box.get_x() + m_r;
	}
	else if (m_x + m_r > box.get_x() + box.get_w() && vx >= 0)
	{
		if (v != NULL)
			v->set_x(-1 * v->get_x());
		m_x = box.get_x() + box.get_w() - m_r;
	}
}

void Sphere::fix_box_y(const Box &box, vector3d *v)
{
	float vy = 0;
	if (v != NULL)
		vy = v->get_y();

	if (m_y - m_r < box.get_y() && vy <= 0)
	{
		if (v != NULL)
			v->set_y(-1 * v->get_y());
		m_y = box.get_y() + m_r;
	}
	else if (m_y + m_r > box.get_y() + box.get_h() && vy >= 0)
	{
		if (v != NULL)
			v->set_y(-1 * v->get_y());
		m_y = box.get_y() + box.get_h() - m_r;
	}
}

void Sphere::fix_box_z(const Box &box, vector3d *v)
{
	float vz = 0;
	if (v != NULL)
		vz = v->get_z();

	if (m_z - m_r < box.get_z() && vz <= 0)
	{
		if (v != NULL)
			v->set_z(-1 * v->get_z());
		m_z = box.get_z() + m_r;
	}
	else if (m_z + m_r > box.get_z() + box.get_d() && vz >= 0)
	{
		if (v != NULL)
			v->set_z(-1 * v->get_z());
		m_z = box.get_z() + box.get_d() - m_r;
	}
}

void Sphere::print() const
{
	printf("sphere: [%f %f %f] %f\n", m_x, m_y, m_z, m_r);
}

void Sphere::make_list()
{
	m_display_list = glGenLists(1);

	glNewList(m_display_list, GL_COMPILE);

	int M = 20;
	int N = 20;
	/*
	M = 10;
	N = 10;
	M = 5;
	N = 5;
	M = 4;
	N = 2;
	*/
	float u_max = 2.0 * M_PI;
	float v_max = M_PI;

	vector3d p[M + 1][N + 1];
	vector3d n[M + 1][N + 1];
	vector2d t[M + 1][N + 1];

	for (int i = 0; i <= M; ++i)
	{
		for (int j = 0; j <= N; ++j)
		{
			float u = (1.0 * i) / M;
			float v = (1.0 * j) / N;

			t[i][j] = vector2d(u, v);

			u *= u_max;
			v *= v_max;

			p[i][j] = vector3d(cos(u) * sin(v), sin(u) * sin(v), cos(v));
			n[i][j] = vector3d(cos(u) * sin(v), sin(u) * sin(v), cos(v));
		}
	}

	int dx[] = { 0, 1, 1, 0 };
	int dy[] = { 1, 1, 0, 0 };

	glBegin(GL_QUADS);

	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				int x = i + dx[k];
				int y = j + dy[k];
				glTexCoord2f(t[x][y][0], t[x][y][1]);
				glNormal3f(n[x][y][0], n[x][y][1], n[x][y][2]);
				glVertex3f(p[x][y][0], p[x][y][1], p[x][y][2]);
			}
		}
	}
	
	glEnd();

	glEndList();

	m_list_made = true;
}

void Sphere::draw() const
{
	if (!m_list_made)
		make_list();

	glPushMatrix();
	glTranslatef(m_x, m_y, m_z);
	glScalef(m_r, m_r, m_r);
	glCallList(m_display_list);
	glPopMatrix();
}



bool Box::m_list_made = false;
int Box::m_display_list = 0;

Box::Box()
	:m_x(0), m_y(0), m_z(0),
	 m_w(0), m_h(0), m_d(0)
{
}

Box::Box(float x, float y, float z,
         float w, float h, float d)
	:m_x(x), m_y(y), m_z(z),
	 m_w(w), m_h(h), m_d(d)
{
}

Box::Box(const vector3d &pos,
         const vector3d &size)
	:m_x(pos[0]),  m_y(pos[1]),  m_z(pos[2]),
	 m_w(size[0]), m_h(size[1]), m_d(size[2])
{
}

const float &Box::operator[](int i) const
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else if (i == 2)
		return m_z;
	else if (i == 3)
		return m_w;
	else if (i == 4)
		return m_h;
	else
		return m_d;
}

float &Box::operator[](int i)
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else if (i == 2)
		return m_z;
	else if (i == 3)
		return m_w;
	else if (i == 4)
		return m_h;
	else
		return m_d;
}

bool Box::collides(const Shape3d *shape3d) const
{
	return shape3d->visit_collides(this);
}

bool Box::visit_collides(const Sphere *sphere) const
{
	return sphere->visit_collides(this);
}

bool Box::visit_collides(const Box *box) const
{
	if (m_w < EPSILON ||
	    m_h < EPSILON ||
	    m_d < EPSILON ||
	    box->m_w < EPSILON ||
	    box->m_h < EPSILON ||
	    box->m_d < EPSILON)
		return false;
	
	return !(
		m_x >= box->m_x + box->m_w ||
		m_y >= box->m_y + box->m_h ||
		m_z >= box->m_z + box->m_d ||
		box->m_x >= m_x + m_w ||
		box->m_y >= m_y + m_h ||
		box->m_z >= m_z + m_d);
}

vector3d Box::get_center() const
{
	return vector3d(m_x + 0.5 * m_w,
	                m_y + 0.5 * m_h,
	                m_z + 0.5 * m_d);
}

const Shape3d &Box::operator+=(const vector3d &rhs)
{
	m_x += rhs[0];
	m_y += rhs[1];
	m_z += rhs[2];
	return *this;
}

void Box::fix_box_x(const Box &box, vector3d *v)
{
	float vx = 0;
	if (v != NULL)
		vx = v->get_x();

	if (m_x < box.get_x() && vx <= 0)
	{
		if (v != NULL)
			v->set_x(-1 * v->get_x());
		m_x = box.get_x();
	}
	else if (m_x + m_w > box.get_x() + box.get_w() && vx >= 0)
	{
		if (v != NULL)
			v->set_x(-1 * v->get_x());
		m_x = box.get_x() + box.get_w() - m_w;
	}
}

void Box::fix_box_y(const Box &box, vector3d *v)
{
	float vy = 0;
	if (v != NULL)
		vy = v->get_y();

	if (m_y < box.get_y() && vy <= 0)
	{
		if (v != NULL)
			v->set_y(-1 * v->get_y());
		m_y = box.get_y();
	}
	else if (m_y + m_h > box.get_y() + box.get_h() && vy >= 0)
	{
		if (v != NULL)
			v->set_y(-1 * v->get_y());
		m_y = box.get_y() + box.get_h() - m_h;
	}
}

void Box::fix_box_z(const Box &box, vector3d *v)
{
	float vz = 0;
	if (v != NULL)
		vz = v->get_z();

	if (m_z < box.get_z() && vz <= 0)
	{
		if (v != NULL)
			v->set_z(-1 * v->get_z());
		m_z = box.get_z();
	}
	else if (m_z + m_d > box.get_z() + box.get_d() && vz >= 0)
	{
		if (v != NULL)
			v->set_z(-1 * v->get_z());
		m_z = box.get_z() + box.get_d() - m_d;
	}
}

void Box::print() const
{
	printf("box: [%f %f %f] [%f %f %f]\n",
	       m_x, m_y, m_z, m_w, m_h, m_d);
}

void Box::make_list()
{
	m_display_list = glGenLists(1);

	glNewList(m_display_list, GL_COMPILE);
	
	int f[6][4] = {
		{ 0, 1, 2, 3 },
		{ 7, 6, 5, 4 },
		{ 1, 5, 6, 2 },
		{ 0, 3, 7, 4 },
		{ 3, 2, 6, 7 },
		{ 1, 0, 4, 5 }
	};
	float n[6][3] = {
		{  0,  0,  1 },
		{  0,  0, -1 },
		{  1,  0,  0 },
		{ -1,  0,  0 },
		{  0,  1,  0 },
		{  0, -1,  0 }
	};
	float v[8][4] = {
		{ 0, 0, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 0, 1, 1 },
		{ 0, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 1, 0 },
		{ 0, 1, 0 }
	};

	for (int i = 0; i < 6; ++i)
	{
		glBegin(GL_POLYGON);
		
		glNormal3f(n[i][0], n[i][1], n[i][2]);
		
		for(int j = 0; j < 4; ++j)
			glVertex3fv(v[f[i][j]]);
		
		glEnd();
	}

	glEndList();
	
	m_list_made = true;
}

void Box::draw() const
{
	if (!m_list_made)
		make_list();

	glPushMatrix();
	glTranslatef(m_x, m_y, m_z);
	glScalef(m_w, m_h, m_d);
	glCallList(m_display_list);

	glPopMatrix();
}

bool Box::contains(float x, float y, float z) const
{
	float min_x = m_x;
	float min_y = m_y;
	float min_z = m_z;
	float max_x = m_x + m_w;
	float max_y = m_y + m_h;
	float max_z = m_z + m_d;
	return
		min_x <= x && x <= max_x &&
		min_y <= y && y <= max_y &&
		min_z <= z && z <= max_z;
}
