#include "hazard.h"

Hazard::Hazard(const string &name)
	:m_name(name)
{
}

Hazard::~Hazard()
{
}

Hazard *Hazard::clone(FILE *,
                      MatLib *)
{
	fprintf(stderr, "Hazard::clone() default method\n");
	fflush(stderr);
	return NULL;
}

void Hazard::update()
{
}

void Hazard::move(float)
{
}

void Hazard::draw() const
{
}



BoxHazard::BoxHazard()
	:Hazard("box")
{
}

Hazard *BoxHazard::clone(FILE *in,
                         MatLib *matlib)
{
	BoxHazard *ret = new BoxHazard();
	char mat[81];
	fscanf(in, "%80s", mat);
	ret->m_material = matlib->get(mat);
	float x, y, z, w, h, d;
	fscanf(in, "%f %f %f %f %f %f", &x, &y, &z, &w, &h, &d);
	ret->m_dims = Box(x, y, z, w, h, d);
	return ret;
}

void BoxHazard::draw() const
{
	m_material->draw();
	m_dims.draw();
	/*
	Surface *dst = camera->get_surface();
	Uint8 c[3];
	SDL_GetRGB(m_color, dst->get_format(),
	           c, c + 1, c + 2);
	GLfloat mat_color[] = { c[0] / 255.0, c[1] / 255.0, c[2] / 255.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_color);

	glBegin(GL_QUADS);

	glVertex3f(m_dims[0], m_dims[1], 0);
	glVertex3f(m_dims[0] + m_dims[2], m_dims[1], 0);
	glVertex3f(m_dims[0] + m_dims[2], m_dims[1] + m_dims[3], 0);
	glVertex3f(m_dims[0], m_dims[1] + m_dims[3], 0);
	
	glEnd();
	
	int x = (int)(m_dims[0] - camera->get_x());
	int y = (int)(m_dims[1] - camera->get_y());
	int w = m_dims[2];
	int h = m_dims[3];
	dst->draw_rect(m_color, Rect(x, y, w, h));
	*/
}

bool BoxHazard::collides(const Shape3d *shape3d) const
{
	return m_dims.collides(shape3d);
}
