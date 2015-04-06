#include "goal.h"

Goal::Goal(FILE *in)
	:m_next(),
	 m_dims(),
	 m_axis()
{
	char next[81];
	fscanf(in, "%80s", next);
	m_next = next;

	float x, y, z, w, h, d;
	fscanf(in, "%f %f %f %f %f %f",
	       &x, &y, &z, &w, &h, &d);
	m_dims = Box(x, y, z, w, h, d);

	fscanf(in, "%f %f %f", &x, &y, &z);
	m_axis = vector3d(x, y, z);

	printf("Goal\n");
	printf("  %s\n", next);
	printf("  %f %f %f\n", x, y, z);
	printf("  %f %f %f\n", w, h, d);
	printf("  %f %f %f\n", m_axis[0], m_axis[1], m_axis[2]);
	fflush(stdout);
}

bool Goal::collides(const Shape3d *shape3d) const
{
	return m_dims.collides(shape3d);
}

void Goal::draw() const
{
	float amb = 0.2;
	float diff = 0.8;
	float spec = 0.5;
	GLfloat mat_ambient[] = { amb, amb, amb, amb };
	GLfloat mat_diffuse[] = { diff, diff, diff, diff };
	GLfloat mat_specular[] = { spec, spec, spec, 0.0 };
	GLfloat mat_shininess[] = { 20.0 };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	m_dims.draw();
}
