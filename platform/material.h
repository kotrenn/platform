#ifndef __MATERIAL_H
#define __MATERIAL_H

#include "vector3d.h"

class Material
{
public:
	Material();
	~Material();

	vector3d get_ambient() const { return m_ambient; }
	vector3d get_diffuse() const { return m_diffuse; }
	vector3d get_specular() const { return m_specular; }
	float get_shininess() const { return m_shininess; }
	void set_ambient(const vector3d &ambient);
	void set_diffuse(const vector3d &diffuse);
	void set_specular(const vector3d &specular);
	void set_shininess(float shininess);

	void draw() const;
	void set_background() const;
private:
	void update_list();
	
	vector3d m_ambient;
	vector3d m_diffuse;
	vector3d m_specular;
	float m_shininess;
	int m_display_list;
	bool m_has_list;
};

#endif
