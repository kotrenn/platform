#include "terrainlib.h"

TerrainLib::TerrainLib()
	:m_types()
{
	m_types['.'] = new TerrainType(true);
}

TerrainLib::~TerrainLib()
{
	for (auto it : m_types)
		delete it.second;
}

void TerrainLib::read_terrain(MatLib *matlib, FILE *in)
{
	char buf[81];
	fscanf(in, "%80s", buf);
	TerrainType *type = new TerrainType(matlib, in);
	m_types[buf[0]] = type;
}

TerrainType *TerrainLib::get(char c)
{
	if (m_types.find(c) == m_types.end())
	{
		fprintf(stderr, "Error: could not find terrain type \'%c\'", c);
		return m_types['.'];
	}
	return m_types[c];
}
