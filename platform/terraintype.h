#ifndef __TERRAIN_TYPE_H
#define __TERRAIN_TYPE_H

#include "matlib.h"
#include "shape3d.h"

class TerrainType
{
public:
	TerrainType(bool invisible);
	TerrainType(MatLib *matlib, FILE *in);
	~TerrainType();

	void draw(const Box &dims);
	bool is_invisible() const { return m_invisible; }
private:
	Material *m_material;
	bool m_invisible;
};

#endif
