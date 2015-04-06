#ifndef __LEVEL_H
#define __LEVEL_H

#include "actor.h"
#include "bullet.h"
#include "camera.h"
#include "common.h"
#include "decoration.h"
#include "goal.h"
#include "hazard.h"
#include "layout.h"
#include "matlib.h"
#include "particle.h"
#include "terrain.h"
#include "terrainlib.h"

class Actor;
class Bullet;
class Camera;
class Decoration;
class Hazard;
class Layout;
class Particle;
class Terrain;
class TerrainLib;

class Level
{
public:
	Level(const string &filename);
	~Level();

	void pause();
	void unpause();

	void update();
	void move(float dt);
	void draw() const;

	string get_name() const { return m_name; }
	int get_w() const { return m_w; }
	int get_h() const { return m_h; }
	int get_d() const { return m_d; }
	Box get_dims() const;
	vector3d get_start() const;
	bool goal(Actor *actor, string *next) const;

	void set_option(const string &option, bool value);
	bool get_option(const string &option) const;

	Camera *get_camera() const { return m_camera; }
	MatLib *get_matlib() const { return m_matlib; }
	
	void add_actor(Actor *actor);
	void add_bullet(Bullet *bullet);
	void add_particle(Particle *particle);
	
	void remove_actor(Actor *actor);
	void remove_bullet(Bullet *bullet);

	bool out_of_bounds(const Actor *actor) const;
	bool out_of_bounds(const Bullet *bullet) const;
	void get_collisions(list<Actor *> *actors,
	                    const Shape3d *shape3d,
	                    int team) const;
	void get_collisions(list<Bullet *> *bullets,
	                    const Shape3d *shape3d,
	                    int team) const;
	Actor *get_next_target(Actor *src, Actor *target,
	                       float dist) const;
	bool hazard(const Actor *actor) const;

	friend class LevelReader;
private:
	void add_lists();
	void clear_lists();

	string m_name;
	int m_w;
	int m_h;
	int m_d;
	string m_background;

	MatLib *m_matlib;

	vector3d m_view_x;
	vector3d m_view_y;

	vector3d m_cell_dims;
	
	map<string, bool> m_options;

	Camera *m_camera;
	int m_front;

	set<Actor *> m_actors;
	set<Actor *> m_actors_add;
	set<Actor *> m_actors_remove;
	
	set<Bullet *> m_bullets;
	set<Bullet *> m_bullets_add;
	set<Bullet *> m_bullets_remove;

	TerrainLib *m_terrain_lib;
	Layout *m_layout;

	//list<Terrain> m_terrain;
	list<Decoration *> m_decorations;
	list<Goal *> m_goals;
	list<Hazard *> m_hazards;

	list<Particle *> m_particles;
};

#endif
