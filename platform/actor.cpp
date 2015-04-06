#include "actor.h"

#include "actorfactory.h"
#include "datamanager.h"
#include "resourcemanager.h"
#include "player.h"

Actor::Actor(const string &name)
	:m_level(NULL),
	 m_name(name),
	 m_sphere(0, 0, 0, 0),
	 m_v(0, 0, 0),
	 m_prev_v(0, 0, 0),
	 m_team(TEAM_ENEMY),
	 m_flying(false),
	 m_jumpable(0),
	 m_jump(0),
	 m_grav(0, 0, 0),
	 m_target(NULL),
	 m_range(10),
	 m_health(1),
	 m_health_max(1),
	 m_dead(false),
	 m_direction(WEST),
	 m_material(NULL),
	 m_frames(0),
	 m_surfaces(),
	 m_frame(0),
	 m_animate(0),
	 m_anim_delay(0),
	 m_invincible(0),
	 m_moved(false),
	 m_parent(NULL)
{
}

Actor::Actor(Level *level, const string &name,
             int frames, float delay)
	:m_level(level),
	 m_name(name),
	 m_sphere(randint(0, level->get_w()),
	          randint(0, level->get_h()),
	          randint(0, level->get_d()),
	          0.8),
	 m_v(0, 0, 0),
	 m_prev_v(0, 0, 0),
	 m_team(TEAM_ENEMY),
	 m_flying(false),
	 m_jumpable(0),
	 m_jump(0),
	 m_grav(0, 0, 0),
	 m_target(NULL),
	 m_range(10),
	 m_health(3),
	 m_health_max(3),
	 m_dead(false),
	 m_direction(WEST),
	 m_material(NULL),
	 m_frames(frames),
	 m_surfaces(),
	 m_frame(0),
	 m_animate(0),
	 m_anim_delay(delay),
	 m_invincible(0),
	 m_moved(false),
	 m_parent(NULL)
{
	ResourceManager *rm = ResourceManager::get_instance();

	m_sphere.print();
	
	for (int i = 0; i < 2 * m_frames; ++i)
	{
		char filename[81];
		snprintf(filename, 80, "actors/%s%d.png", name.c_str(), i + 1);
		Surface *surface = rm->get_image(filename);
		m_surfaces.push_back(surface);
	}

	load_material();
}

Actor::~Actor()
{
}

Actor *Actor::clone(Level *, const string &, FILE *)
{
	fprintf(stderr, "Actor::clone() default method\n");
	return NULL;
}

void Actor::pause()
{
	m_invincible.pause();
}

void Actor::unpause()
{
	m_invincible.unpause();
}

void Actor::reset_update()
{
	m_moved = false;
}

void Actor::update()
{
	if (m_target == NULL)
		select_target();
	else
	{
		DataManager *dm = DataManager::get_instance();
		int max_dist = dm->get_int("view_dist");
		vector3d center = m_target->get_center();
		vector3d disp = m_sphere.get_center() - center;
		if (disp * disp > max_dist * max_dist)
			m_target = NULL;
		else if (!m_flying)
			m_direction = m_sphere[0] < center[0]? EAST : WEST;
	}
	
	if (m_animate.done())
	{
		bool herp = true;
		if (m_flying)
			herp = true;
		if (m_target != NULL && !m_flying)
			herp = false;
		if (!m_jump.done())
			herp = false;
		if (fabs(m_v[0]) > EPSILON)
		    herp = false;
		if (!herp)
		{
			m_frame = (m_frame + 1) % m_frames;
			m_animate.reset(m_anim_delay);
		}
	}
}

void Actor::reset_jumpable()
{
	DataManager *dm = DataManager::get_instance();
	float buffer = dm->get_flt("jump_buffer");
	m_jumpable.reset(buffer);
}

void Actor::move(float dt)
{
	if (m_moved)
		return;

	if (m_parent != NULL)
		if (!m_parent->has_moved())
			m_parent->move(dt);

	

	/* actual moving stuff */
	
	if (m_target == NULL || m_flying)
		m_direction = m_v[0] > 0? EAST : WEST;
	if (m_v * m_v > 0)
		m_prev_v = m_v;

	DataManager *dm = DataManager::get_instance();
	float gravity = dm->get_flt("gravity");
	float jump = dm->get_flt("jump_speed");
	Camera *camera = m_level->get_camera();
	//gravity = 0;

	if (!m_flying)
	{
		//if (jumping)
		/*
		if (m_jump > 0)
		{
			m_jump--;
			//m_v[1] = -jump;
			//m_v += jump * camera->get_view_y();
		}
		*/
	}

	m_sphere += dt * m_v;

	if (!m_flying)
	{
		vector3d g(0, 0, 0);
		g = -gravity * camera->get_view_y();
		//if (jumping)
		//m_falling = true;
		if (!m_jump.done())
		{
			g = jump * camera->get_view_y();
		}
			
		//m_v.set_y(gravity);
		//m_v[1] = gravity;
		//m_sphere += -gravity * camera->get_view_y() * dt;
		//m_sphere += 0.5 * g * dt * dt;
		m_grav += g * dt;

		float max_grav = dm->get_flt("max_grav");
		if (m_grav * m_grav > max_grav * max_grav)
		{
			m_grav.normalize();
			m_grav *= max_grav;
		}
		
		m_sphere += m_grav * dt;
	}
	//m_sphere.fix_box(m_level->get_dims());

	

	/* mark that we're done */

	m_moved = true;
}

void Actor::draw() const
{
	/*
	glLineWidth(5.0);

	float mat_color[] = { 0.0, 0.0, 0.0, 1.0 };
	if (m_grav.norm() > 0)
		mat_color[0] = 1.0;
	if (!m_jumpable.done())
		mat_color[1] = 1.0;
	if (!m_jump.done())
		mat_color[2] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_color);
	*/

	int t = (int)(5 * m_invincible.get_time());
	if (t % 2 == 0 && !m_invincible.done())
		return;

	m_material->draw();
	m_sphere.draw();
	
	m_level->get_matlib()->get("health")->draw();

	Camera *camera = m_level->get_camera();
	vector3d view_x = camera->get_view_x();
	vector3d view_y = camera->get_view_y();
	vector3d view_z = view_x % view_y;

	for (int i = 0; i < m_health; ++i)
	{
		float theta = i / (m_health_max - 1.0);
		theta *= 180.0;
		float r = 1.5 * m_sphere.get_r();
		vector3d pos = r * view_x;
		pos = view_z.rotate(theta, pos);
		pos += m_sphere.get_center();
		float size = 0.4 * m_sphere.get_r();
		vector3d dims = size * vector3d(1, 1, 1);
		vector3d center = pos - 0.5 * dims;
		Box box(center, dims);
		box.draw();
	}

	return;

	/*
	Surface *dst = camera->get_surface();
	int x = (int)(m_circle[0] - camera->get_x());
	int y = (int)(m_circle[1] - camera->get_y());
	int r = (int)m_circle[2];
	if (x + r < 0 ||
	    y + r < 0 ||
	    x - r > camera->get_w() ||
	    y - r > camera->get_h())
		return;
	
	int t = (int)(5 * m_invincible.get_time());
	if (t % 2 == 0 && !m_invincible.done())
		return;

	if (m_frames == 0)
		dst->draw_circle(mapRGB(255, 255, 255), x, y, r, 1);
	else
	{
		int frame = m_frame;
		if (m_direction == EAST)
			frame += m_frames;
		Surface *surface = m_surfaces[frame];
		y += r;
		y -= surface->get_h();
		x -= 0.5 * surface->get_w();
		dst->blit(surface, x, y);
	}
	*/
}

void Actor::explode() const
{
	ResourceManager *rm = ResourceManager::get_instance();
	rm->play_sound("explode.wav");
	
	DataManager *dm = DataManager::get_instance();

	int num = dm->get_int("num_particles");
	for (int i = 0; i < num; ++i)
	{
		float theta = randint(1, 360) * M_PI / 180.0;
		float phi   = randint(1, 180) * M_PI / 180.0;
		float speed = 30 * 0.2 * randint(2, 8);
		vector3d v(cos(theta) * sin(phi),
		           sin(theta) * sin(phi),
		           cos(phi));
		v *= speed;
		Sphere pos = m_sphere;
		pos[3] = 0.05 * randint(3, 8);
		float time = 0.05 * randint(10, 40);
		new Particle(m_level, mapRGB(0, 0, 255), pos, v, time);
	}
}

void Actor::damage()
{
	if (!m_invincible.done())
		return;

	DataManager *dm = DataManager::get_instance();
	ResourceManager *rm = ResourceManager::get_instance();

	int pain = 1;
	if (m_team == TEAM_PLAYER && false)
		pain = 20;
	m_health -= pain;
	printf("m_health = %d\n", m_health);
	if (m_team == TEAM_PLAYER)
		m_invincible.reset(dm->get_flt("invincible"));
	if (m_health <= 0 && !m_dead)
	{
		die();
	}
	else if (!m_dead)
	{
		if (m_team == TEAM_PLAYER)
			rm->play_sound("player_hit.wav");
		else
			rm->play_sound("enemy_hit.wav");
	}
}

void Actor::die()
{
	explode();
	m_dead = true;
	
	// drop an item upon death
	/*
	int k = 2;
	if (m_level->subspace())
		k = 4;

	PlayerActor *player = (PlayerActor *)m_level->get_player();

	int p = randint(1, k * m_item_rate);
	if ((p == 1 || m_level->get_boss() == this) && m_items)
	{
		vector3d center = get_center();
		int n = randint(0, 5);
		int t = randint(1, 60);
		if (player != NULL)
			if (player->subspace(0))
				t = randint(1, 50);
		if (m_level->get_boss() == this)
			t = randint(1, 11);
		if (t <= 6)
			n = ITEM_BLANK;
		else if (t <= 7)
			n = ITEM_HEALTH;
		else if (t <= 8)
			n = ITEM_SHIELD;
		else if (t <= 9)
			n = ITEM_BOMB;
		else if (t <= 10)
			n = ITEM_HEALTH_PLUS;
		else if (t <= 11)
			n = ITEM_SHIELD_PLUS;
		new Item(m_level, center, n);
	}

	// spawn an enemy upon death
	if (color_match(m_color, m_prev_hit_color) &&
	    m_death_spawn.size() > 0)
	{
		ActorFactory *af = ActorFactory::get_instance();
		Actor *actor = af->load_actor(m_level, m_death_spawn);
		m_level->add_actor(actor);
	}
	*/
	
	m_level->remove_actor(this);
}

void Actor::set_center(const vector3d &center)
{
	m_sphere[0] = center[0];
	m_sphere[1] = center[1];
	m_sphere[2] = center[2];
}

void Actor::set_level(Level *level)
{
	m_level = level;
	m_level->add_actor(this);
	m_target = NULL;
	load_material();
}

void Actor::load_material()
{
	string matname = "actor_" + m_name;
	MatLib *matlib = m_level->get_matlib();
	m_material = matlib->get(matname);
}

void Actor::actor_dying(Actor *actor)
{
	if (m_target == actor)
		m_target = NULL;
}

void Actor::select_target()
{
	m_target = m_level->get_next_target(this, m_target, m_range);
}



StandActor::StandActor()
	:Actor("stand")
{
}

StandActor::StandActor(Level *level, const string &name)
	:Actor(level, name, 3, 0.3)
{
}

Actor *StandActor::clone(Level *level,
                         const string &name,
                         FILE *in)
{
	StandActor *ret = new StandActor(level, name);
	float x, y, z;
	fscanf(in, "%f %f %f", &x, &y, &z);
	ret->m_sphere = Sphere(x, y, z, 0.8);
	ret->m_start = vector3d(x, y, z);
	ret->reset_health(3);
	fscanf(in, "%f", &(ret->m_speed));
	fscanf(in, "%f", &(ret->m_range));
	fscanf(in, "%f %f %f", &x, &y, &z);
	ret->m_local_x = vector3d(x, y, z);
	ret->m_local_x.normalize();
	
	ret->m_v = ret->m_speed * ret->m_local_x;
	if (randint(1, 2) == 1)
		ret->m_v *= -1;
	
	ret->m_timer.reset(0);
	
	return ret;
}

void StandActor::update()
{
	Actor::update();
	
	if (m_target != NULL)
	{
		//m_v = vector3d(0, 0, 0);
		if (m_timer.done())
		{
			create_bullet();
			m_timer.reset(0.6);
		}
	}
	//else
	if (true)
	{
		if (fabs(m_v * m_local_x) < EPSILON)
		{
			float speed = randint(1, 2) == 1? m_speed : -m_speed;
			m_v += speed * m_local_x;
		}
		vector3d center = get_center();
		vector3d sphere = (center * m_local_x) * m_local_x;
		vector3d start = (m_start * m_local_x) * m_local_x;
		if ((sphere - start).norm() > m_range)
			m_v -= 2.0 * (m_v * m_local_x) * m_local_x;
	}
}

void StandActor::create_bullet()
{
	DataManager *dm = DataManager::get_instance();
	vector3d m_center = get_center();
	vector3d center = m_target->get_center();

	vector3d dir = center - m_center;
	if (dir * dir < EPSILON)
		return;
	dir.normalize();

	float x = m_sphere[0];
	float y = m_sphere[1];
	float z = m_sphere[2];
	float r = 0.4 * m_sphere.get_r();
	Sphere sphere(x, y, z, r);
	float s = dm->get_flt("pbullet_speed");
	vector3d v = s * dir;
	new Bullet(this, new SphereBullet(sphere, v));

	ResourceManager *rm = ResourceManager::get_instance();
	rm->play_sound("stand.wav");
}



FlyActor::FlyActor()
	:Actor("fly")
{
}

FlyActor::FlyActor(Level *level, const string &name)
	:Actor(level, name, 3, 0.3)
{
}

Actor *FlyActor::clone(Level *level,
                         const string &name,
                         FILE *in)
{
	FlyActor *ret = new FlyActor(level, name);
	float x, y, z;
	fscanf(in, "%f %f %f", &x, &y, &z);
	ret->m_sphere = Sphere(x, y, z, 0.8);
	ret->m_start = vector3d(x, y, z);
	ret->reset_health(5);
	fscanf(in, "%f", &(ret->m_speed));
	fscanf(in, "%f", &(ret->m_range));
	fscanf(in, "%f %f %f", &x, &y, &z);
	ret->m_local_x = vector3d(x, y, z);
	ret->m_local_x.normalize();
	
	ret->m_v = ret->m_speed * ret->m_local_x;
	if (randint(1, 2) == 1)
		ret->m_v *= -1;
	
	ret->m_timer.reset(0);
	ret->m_flying = true;
	return ret;
}

void FlyActor::update()
{
	Actor::update();

	if (m_target != NULL)
	{
		if (m_timer.done())
		{
			create_bullet();
			m_timer.reset(0.3);
		}
	}

	if (fabs(m_v * m_local_x) < EPSILON)
	{
		float speed = randint(1, 2) == 1? m_speed : -m_speed;
		m_v += speed * m_local_x;
	}
	vector3d center = get_center();
	vector3d sphere = (center * m_local_x) * m_local_x;
	vector3d start = (m_start * m_local_x) * m_local_x;
	if ((sphere - start).norm() > m_range)
		m_v -= 2.0 * (m_v * m_local_x) * m_local_x;
}

void FlyActor::create_bullet()
{
	DataManager *dm = DataManager::get_instance();
	vector3d m_center = get_center();
	vector3d center = m_target->get_center();

	vector3d dir = center - m_center;
	dir.normalize();

	float x = m_sphere[0];
	float y = m_sphere[1];
	float z = m_sphere[2];
	float r = 0.4 * m_sphere.get_r();
	Sphere sphere(x, y, z, r);
	float s = dm->get_flt("pbullet_speed");
	vector3d v = s * dir;
	new Bullet(this, new SphereBullet(sphere, v));

	ResourceManager *rm = ResourceManager::get_instance();
	rm->play_sound("fly.wav");
}



WallActor::WallActor()
	:Actor("wall")
{
}

WallActor::WallActor(Level *level, const string &name)
	:Actor(level, name, 1, 1.0)
{
}

Actor *WallActor::clone(Level *level,
                         const string &name,
                         FILE *in)
{
	WallActor *ret = new WallActor(level, name);
	float x, y, z;
	fscanf(in, "%f %f %f", &x, &y, &z);
	ret->m_sphere = Sphere(x, y, z, 5);
	ret->reset_health(10);
	ret->m_v = vector3d(0, 0, 0);
	ret->m_timer.reset(0);
	ret->m_flying = true;
	ret->m_range = 30;
	return ret;
}

void WallActor::update()
{
	Actor::update();

	if (m_target != NULL)
	{
		if (m_timer.done())
		{
			create_bullet();
			m_timer.reset(2.0);
		}
	}
}

void WallActor::create_bullet()
{
	DataManager *dm = DataManager::get_instance();
	vector3d m_center = get_center();
	vector3d center = m_target->get_center();

	vector3d dir = center - m_center;
	if (dir * dir < EPSILON)
		return;
	dir.normalize();

	float x = m_sphere[0];
	float y = m_sphere[1];
	float z = m_sphere[2];
	float r = 0.32;
	Sphere sphere(x, y, z, r);
	float speed = dm->get_flt("pbullet_speed");
	vector3d v = speed * dir;
	new Bullet(this, new SphereBullet(sphere, v));
	vector3d axis = v;
	axis.normalize();
	vector3d foo(axis[1], axis[2], axis[0]);
	foo = foo % axis;
	foo.normalize();
	for (int i = 0; i < 4; ++i)
	{
		vector3d vp = foo.rotate(15, v);
		vp = axis.rotate(90 * i, vp);
		new Bullet(this, new SphereBullet(sphere, vp));
	}

	ResourceManager *rm = ResourceManager::get_instance();
	rm->play_sound("wall.wav");
}



BombActor::BombActor()
	:Actor("bomb")
{
}

BombActor::BombActor(Level *level, const string &name)
	:Actor(level, name, 1, 1.0)
{
}

Actor *BombActor::clone(Level *level,
                         const string &name,
                         FILE *in)
{
	BombActor *ret = new BombActor(level, name);
	float x, y, z;
	fscanf(in, "%f %f %f", &x, &y, &z);
	ret->m_center = vector3d(x, y, z);
	ret->m_sphere = Sphere(x, y, z, 10);
	ret->reset_health(10);
	ret->m_v = vector3d(0, 0, 0);
	ret->m_timer.reset(0);
	ret->m_flying = true;
	ret->m_range = 9001.0;
	fscanf(in, "%f %f %f", &x, &y, &z);
	ret->m_axis = vector3d(x, y, z);
	ret->m_axis.normalize();
	fscanf(in, "%f %f %f", &x, &y, &z);
	ret->m_up = vector3d(x, y, z);
	ret->m_up.normalize();
	fscanf(in, "%f", &(ret->m_radius));
	fscanf(in, "%f", &(ret->m_theta));
	fscanf(in, "%f", &(ret->m_omega));
	ret->m_timer.reset(1.0);
	
	return ret;
}

void BombActor::update()
{
	Actor::update();

	if (m_target != NULL)
	{
		if (m_timer.done())
		{
			create_bullet();
			m_timer.reset(4.0);
		}
	}
}

void BombActor::move(float dt)
{
	m_theta += m_omega * dt;
	while (m_theta > 360.0)
		m_theta -= 360.0;

	vector3d pos = m_radius * m_up;
	pos = m_axis.rotate(m_theta, pos);
	pos += m_center;
	m_sphere.set_center(pos);
}

void BombActor::create_bullet()
{
	DataManager *dm = DataManager::get_instance();
	vector3d m_center = get_center();
	vector3d center = m_target->get_center();

	vector3d dir = center - m_center;
	if (dir * dir < EPSILON)
		return;
	dir.normalize();

	float x = m_sphere[0];
	float y = m_sphere[1];
	float z = m_sphere[2];
	float r = 0.6 * m_sphere.get_r();
	Sphere sphere(x, y, z, r);
	//float speed = dm->get_flt("pbullet_speed");
	float speed = 10.0;
	vector3d v = speed * dir;
	printf("v = %f %f %f\n", v[0], v[1], v[2]);
	new Bullet(this, new SphereBullet(sphere, v));
	
	ResourceManager *rm = ResourceManager::get_instance();
	rm->play_sound("bomb3.wav");
}
