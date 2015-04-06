#ifndef __ACTOR_H
#define __ACTOR_H

#include "common.h"
#include "camera.h"
#include "level.h"
#include "material.h"
#include "timer.h"

#define TEAM_ENEMY 0
#define TEAM_PLAYER 1

typedef enum
{
	WEST = 0,
	EAST = 1
} Direction;

class Camera;
class Level;

class Actor
{
public:
	Actor(const string &name);
	Actor(Level *level, const string &name,
	      int frames, float delay);
	virtual ~Actor();

	virtual Actor *clone(Level *level,
	                     const string &name,
	                     FILE *in);

	string get_name() const { return m_name; }

	virtual void pause();
	virtual void unpause();

	virtual void reset_update();
	virtual void update();
	virtual void move(float dt);
	virtual void draw() const;

	bool has_moved() const { return m_moved; }
	void set_parent(Actor *parent) { m_parent = parent; }
	Actor *get_parent() const { return m_parent; }

	void explode() const;
	virtual void damage();
	bool dead() const { return m_dead; }
	virtual void die();

	void set_level(Level *level);
	void set_target(Actor *target) { m_target = target; }
	virtual void actor_dying(Actor *);

	Level *get_level() const { return m_level; }
	int get_team() const { return m_team; }
	Sphere get_sphere() const { return m_sphere; }
	vector3d get_center() const { return m_sphere.get_center(); }
	float get_radius() const { return m_sphere.get_r(); }
	vector3d get_v() const { return m_v; }

	void set_center(const vector3d &center);
	void set_v(const vector3d &v) { m_v = v; }
	void set_jump(float jump) { m_jump.reset(jump); }
	void clear_jump() { m_jump.clear(); }
	void reset_jumpable();
	void reset_grav() { m_grav = vector3d(0, 0, 0); }

	void reset_health(int health) { m_health = m_health_max = health; }
protected:
	void select_target();
	void load_material();

	Level *m_level;
	string m_name;
	Sphere m_sphere;
	vector3d m_v;
	vector3d m_prev_v;
	int m_team;
	bool m_flying;
	Timer m_jumpable;
	Timer m_jump;
	vector3d m_grav;
	Actor *m_target;
	float m_range;
	int m_health;
	int m_health_max;
	bool m_dead;
	Direction m_direction;
	Material *m_material;
	int m_frames;
	vector<Surface *> m_surfaces;
	int m_frame;
	Timer m_animate;
	float m_anim_delay;
	Timer m_invincible;
	
	bool m_moved;
	Actor *m_parent;
};

class StandActor : public Actor
{
public:
	StandActor();
	StandActor(Level *level, const string &name);

	Actor *clone(Level *level,
	             const string &name,
	             FILE *in);

	void update();
private:
	void create_bullet();
	vector3d m_start;
	float m_speed;
	Timer m_timer;
	float m_range;
	vector3d m_local_x;
};

class FlyActor : public Actor
{
public:
	FlyActor();
	FlyActor(Level *level, const string &name);

	Actor *clone(Level *level,
	             const string &name,
	             FILE *in);

	void update();
private:
	void create_bullet();
	vector3d m_start;
	float m_speed;
	Timer m_timer;
	float m_range;
	vector3d m_local_x;
};

class WallActor : public Actor
{
public:
	WallActor();
	WallActor(Level *level, const string &name);

	Actor *clone(Level *level,
	             const string &name,
	             FILE *in);

	void update();
private:
	void create_bullet();
	Timer m_timer;
};

class BombActor : public Actor
{
public:
	BombActor();
	BombActor(Level *level, const string &name);

	Actor *clone(Level *level,
	             const string &name,
	             FILE *in);

	void update();
	void move(float dt);
private:
	void create_bullet();
	vector3d m_center;
	vector3d m_axis;
	vector3d m_up;
	float m_radius;
	float m_theta;
	float m_omega;
	Timer m_timer;
};

#endif
