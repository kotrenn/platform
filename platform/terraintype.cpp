#include "terraintype.h"

TerrainType::TerrainType(bool invisible)
	:m_material(NULL),
	 m_invisible(invisible)
{
}

TerrainType::TerrainType(MatLib *matlib, FILE *in)
	:m_material(NULL),
	 m_invisible(false)
{
	char buf[81] = "";
	while (strcmp(buf, "done") != 0)
	{
		fscanf(in, "%80s", buf);
		if (strcmp(buf, "usemtl") == 0)
		{
			fscanf(in, "%80s", buf);
			m_material = matlib->get(buf);
		}
		else if (strcmp(buf, "invisible") == 0)
			m_invisible = true;
	}
}

TerrainType::~TerrainType()
{
}

void TerrainType::draw(const Box &dims)
{
	if (m_material != NULL)
		m_material->draw();
	dims.draw();
}
