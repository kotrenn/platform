#include "material.h"

Material::Material()
	:m_ambient(),
	 m_diffuse(),
	 m_specular(),
	 m_shininess(0),
	 m_display_list(0),
	 m_has_list(false)
{
	update_list();
}

Material::~Material()
{
}

void Material::set_ambient(const vector3d &ambient)
{
	m_ambient = ambient;
	update_list();
}

void Material::set_diffuse(const vector3d &diffuse)
{
	m_diffuse = diffuse;
	update_list();
}

void Material::set_specular(const vector3d &specular)
{
	m_specular = specular;
	update_list();
}

void Material::set_shininess(float shininess)
{
	m_shininess = shininess;
	update_list();
}

void Material::update_list()
{
	/*
	if (m_has_list)
	{
		glDeleteLists(m_display_list, 1);
		m_has_list = false;
	}
	*/

	GLfloat mat_ambient[] = { m_ambient[0],
	                          m_ambient[1],
	                          m_ambient[2],
	                          1.0 };
	GLfloat mat_diffuse[] = { m_diffuse[0],
	                          m_diffuse[1],
	                          m_diffuse[2],
	                          1.0 };
	GLfloat mat_specular[] = { m_specular[0],
	                           m_specular[1],
	                           m_specular[2],
	                           1.0 };
	GLfloat mat_shininess[] = { m_shininess };
	

	if (!m_has_list)
		m_display_list = glGenLists(1);
	
	glNewList(m_display_list, GL_COMPILE);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	glEndList();
	

	m_has_list = true;
}

void Material::draw() const
{
	glCallList(m_display_list);
}

void Material::set_background() const
{
	glClearColor(m_ambient[0], m_ambient[1], m_ambient[2], 1.0);
}
