#ifndef __LAYOUT_H
#define __LAYOUT_H

#include "actor.h"
#include "material.h"
#include "terrain.h"
#include "terrainlib.h"

class Level;
class Terrain;
class TerrainLib;

class Layout
{
public:
	Layout(Level *level,
	       const vector3d &cell_dims,
	       TerrainLib *terrain_lib,
	       FILE *in);
	~Layout();

	void fix(Actor *actor, Camera *camera) const;
	bool collides(Bullet *bullet) const;

	void draw(bool clip = false, int depth = 0);

	vector3d get_start() const { return m_start; }
private:
	Terrain *get(int x, int y, int z) const;
	void set_start(int x, int y, int z);
	Level *m_level;
	vector3d m_cell_dims;
	int m_w;
	int m_h;
	int m_d;
	Terrain *m_terrain;
	vector3d m_start;
};

#endif
