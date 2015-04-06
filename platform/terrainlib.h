#ifndef __TERRAIN_LIB_H
#define __TERRAIN_LIB_H

#include "terrain.h"
#include "terraintype.h"

class TerrainLib
{
public:
	TerrainLib();
	~TerrainLib();

	void read_terrain(MatLib *matlib, FILE *in);
	TerrainType *get(char c);
private:
	map<char, TerrainType *> m_types;
};

#endif
