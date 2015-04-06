#ifndef __TERRAIN_H
#define __TERRAIN_H

#include "actor.h"
#include "bullet.h"
#include "camera.h"
#include "terraintype.h"

class Actor;
class Bullet;
class Camera;

class Terrain
{
public:
	Terrain();
	Terrain(const Box &dims,
	        TerrainType *type);
	~Terrain();

	void fix(Actor *actor, Camera *camera) const;
	bool collides(const Bullet *bullet) const;

	void draw() const;
private:
	void line_fix(Actor *actor,
	              const vector2d &p1,
	              const vector2d &p2,
	              bool jump, bool bottom) const;
	TerrainType *m_type;
	Box m_dims;
};

#endif
