#include "level.h"

#include "actorfactory.h"
#include "datamanager.h"
#include "levelreader.h"
#include "player.h"
#include "resourcemanager.h"

Level::Level(const string &filename)
	:m_name(filename),
	 m_w(0), m_h(0), m_d(0),
	 m_background(""),
	 m_matlib(new MatLib()),
	 m_view_x(1, 0, 0),
	 m_view_y(0, 1, 0),
	 m_cell_dims(0, 0, 0),
	 m_options(),
	 m_camera(NULL),
	 m_front(0),
	 m_actors(), m_actors_add(), m_actors_remove(),
	 m_bullets(), m_bullets_add(), m_bullets_remove(),
	 m_terrain_lib(new TerrainLib()),
	 m_layout(NULL),
	 m_decorations(),
	 m_goals(),
	 m_hazards(),
	 m_particles()
{
	printf("Level::Level()\n");
	fflush(stdout);
	
	LevelReader().read(this, filename);

	System *system = System::get_instance();
	int w, h;
	system->get_size(&w, &h);
	m_camera = new Camera(m_layout->get_start(),
	                      vector2d(w, h),
	                      m_view_x,
	                      m_view_y);

	m_matlib->get(m_background)->set_background();
}

Level::~Level()
{
	printf("Level::~Level()\n");
	fflush(stdout);
	
	add_lists();

	for (auto a : m_actors)
		remove_actor(a);
	for (auto b : m_bullets)
		remove_bullet(b);

	m_actors.clear();
	m_bullets.clear();

	clear_lists();

	while (m_decorations.size() > 0)
	{
		delete m_decorations.back();
		m_decorations.pop_back();
	}

	while (m_goals.size() > 0)
	{
		delete m_goals.back();
		m_goals.pop_back();
	}

	while (m_hazards.size() > 0)
	{
		delete m_hazards.back();
		m_hazards.pop_back();
	}

	while (m_particles.size() > 0)
	{
		delete m_particles.back();
		m_particles.pop_back();
	}

	delete m_layout;
	delete m_terrain_lib;
	delete m_camera;
	delete m_matlib;
}

void Level::pause()
{
	for (auto a : m_actors)
		a->pause();
	for (auto b : m_bullets)
		b->pause();
	for (auto d : m_decorations)
		d->pause();
	for (auto h : m_hazards)
		h->pause();
	for (auto p : m_particles)
		p->pause();
}

void Level::unpause()
{
	for (auto a : m_actors)
		a->unpause();
	for (auto b : m_bullets)
		b->unpause();
	for (auto d : m_decorations)
		d->unpause();
	for (auto h : m_hazards)
		h->pause();
	for (auto p : m_particles)
		p->pause();
}

void Level::update()
{
	if (m_camera->is_moving())
		return;
	
	for (auto a : m_actors)
		a->reset_update();

	for (auto a : m_actors)
		a->update();
	for (auto b : m_bullets)
		b->update();
	
	// check for stuff out of bounds here
	for (auto a : m_actors)
		if (out_of_bounds(a))
			a->die();
	for (auto b : m_bullets)
		if (out_of_bounds(b))
			remove_bullet(b);

	add_lists();
	clear_lists();

	for (auto d : m_decorations)
		d->update();

	for (auto h : m_hazards)
		h->update();

	// clear out dead particles
	auto p = m_particles.begin();
	while (p != m_particles.end())
	{
		(*p)->update();
		if ((*p)->dead())
		{
			Particle *particle = *p;
			p = m_particles.erase(p);
			delete particle;
		}
		else
			p++;
	}
}

void Level::add_lists()
{
	for (auto a : m_actors_add)
		m_actors.insert(a);
	for (auto b : m_bullets_add)
		m_bullets.insert(b);

	m_actors_add.clear();
	m_bullets_add.clear();
}

void Level::clear_lists()
{
	for (auto a : m_actors_remove)
	{
		for (auto a2 : m_actors)
			a2->actor_dying(a);
		for (auto b : m_bullets)
			b->actor_dying(a);
	}

	for (auto a : m_actors_remove)
	{
		m_actors.erase(a);
		if (a->get_team() != TEAM_PLAYER)
			delete a;
	}

	for (auto b : m_bullets_remove)
	{
		m_bullets.erase(b);
		delete b;
	}

	m_actors_remove.clear();
	m_bullets_remove.clear();
}

void Level::move(float dt)
{
	m_camera->move(dt);

	if (m_camera->is_moving())
		return;

	for (auto a : m_actors)
		a->move(dt);
	for (auto b : m_bullets)
		b->move(dt);

	for (auto a : m_actors)
		m_layout->fix(a, m_camera);
	for (auto b : m_bullets)
		if (m_layout->collides(b))
			remove_bullet(b);

	for (auto h : m_hazards)
		h->move(dt);

	for (auto p : m_particles)
		p->move(dt);
}

void Level::draw() const
{
	m_camera->draw();

	if (m_actors.size() == 1 || !get_option("boss"))
		for (auto g : m_goals)
			g->draw();

	for (auto h : m_hazards)
		h->draw();

	/*
	for (auto d : m_decorations)
		d->draw();
	*/

	m_layout->draw(get_option("clip"), m_front);

	for (auto b : m_bullets)
		b->draw();
	for (auto a : m_actors)
		a->draw();

	for (auto p : m_particles)
		p->draw();
	
	
	vector3d pos = m_camera->get_eye();
	
	GLfloat light_position0[] = { 1.0, -1.0, -1.0, 0.0 };
	GLfloat light_position1[] = { pos[0], pos[1], pos[2], 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
}

void Level::set_option(const string &option, bool value)
{
	m_options[option] = value;
}

bool Level::get_option(const string &option) const
{
	if (m_options.find(option) == m_options.end())
		return false;
	else
		return (*m_options.find(option)).second;
}

Box Level::get_dims() const
{
	return Box(0, 0, 0, m_w, m_h, m_d);
}

vector3d Level::get_start() const
{
	return m_layout->get_start();
}

bool Level::goal(Actor *actor, string *next) const
{
	bool hidden_goals = true;
	if (m_actors.size() == 1)
		hidden_goals = false;
	if (get_option("boss") == false)
		hidden_goals = false;

	if (hidden_goals)
		return false;
	
	Sphere sphere = actor->get_sphere();
	for (auto g : m_goals)
	{
		if (g->collides(&sphere))
		{
			printf("collision!\n");
			*next = g->get_next();
			return true;
		}
	}

	return false;
}

void Level::add_actor(Actor *actor)
{
	m_actors_add.insert(actor);
	
	if (actor->get_team() != TEAM_PLAYER)
		return;
	
	for (int i = 0; i < 20; ++i)
	{
		float theta = randint(1, 360);
		float z = 0.2 * randint(0, 20);
		float speed = -0.3 * randint(2, 6);
		vector3d view_x = m_camera->get_view_x();
		vector3d view_y = m_camera->get_view_y();
		vector3d view_z = view_x % view_y;
		vector3d pos = (m_cell_dims * view_x) * view_x;
		pos = view_y.rotate(theta, pos);
		pos += z * view_y;
		pos += actor->get_center();
		float r = (m_cell_dims * view_x) * 0.03 * randint(2, 4);
		Sphere sphere(pos, r);
		vector3d v = speed * view_y;
		float time = 0.05 * randint(10, 40);
		new Particle(this, mapRGB(255, 255, 255), sphere, v, time);
	}
}

void Level::add_bullet(Bullet *bullet)
{
	m_bullets_add.insert(bullet);
}

void Level::add_particle(Particle *particle)
{
	m_particles.push_back(particle);
}

void Level::remove_actor(Actor *actor)
{
	m_actors_remove.insert(actor);
}

void Level::remove_bullet(Bullet *bullet)
{
	m_bullets_remove.insert(bullet);
}

bool Level::out_of_bounds(const Actor *actor) const
{
	int buf = 5;
	float w = (m_w + 2 * buf) * m_cell_dims.get_x();
	float h = (m_h + 2 * buf) * m_cell_dims.get_y();
	float d = (m_d + 2 * buf) * m_cell_dims.get_z();
	Box bounds(-buf * m_cell_dims, vector3d(w, h, d));
	Sphere sphere = actor->get_sphere();
	return !bounds.collides(&sphere);
}

bool Level::out_of_bounds(const Bullet *bullet) const
{
	float w = m_w * m_cell_dims.get_x();
	float h = m_h * m_cell_dims.get_y();
	float d = m_d * m_cell_dims.get_z();
	Box bounds(0, 0, 0, w, h, d);
	Shape3d *shape3d = bullet->get_shape3d();
	return !bounds.collides(shape3d);
}

void Level::get_collisions(list<Actor *> *actors,
                           const Shape3d *shape3d,
                           int team) const
{
	for (auto a : m_actors)
		if (a->get_team() != team)
			if (a->get_sphere().collides(shape3d))
				actors->push_back(a);
}

void Level::get_collisions(list<Bullet *> *bullets,
                           const Shape3d *shape3d,
                           int team) const
{
	for (auto b : m_bullets)
		if (b->get_team() != team)
			if (b->get_shape3d()->collides(shape3d))
				bullets->push_back(b);
}

Actor *Level::get_next_target(Actor *src, Actor *target,
                              float dist) const
{
	Actor *ret = NULL;

	bool enemies = false;
	for (auto a : m_actors)
		if (a->get_team() != src->get_team())
			if ((src->get_center() - a->get_center()).norm() < dist)
				enemies = true;

	if (!enemies)
		return NULL;

	bool has_target = false;
	for (auto a : m_actors)
		if (a == target)
			has_target = true;

	if (!has_target)
		target = NULL;

	float cur_dist;
	if (target == NULL)
	{
		auto a = m_actors.begin();
		bool first = true;
		do
		{
			if (!first)
				a++;
			first = false;
			ret = *a;
			vector2d disp = src->get_center() - (*a)->get_center();
			cur_dist = disp.norm();
		} while ((*a)->get_team() == src->get_team() ||
		         cur_dist > dist);
	}
	else
	{
		auto target_it = m_actors.begin();
		for (auto a = m_actors.begin(); a != m_actors.end(); a++)
			if (*a == target)
				target_it = a;

		auto a = target_it;
		do
		{
			a++;
			if (a == m_actors.end())
				a = m_actors.begin();
			vector2d disp = src->get_center() - (*a)->get_center();
			cur_dist = disp.norm();
		} while ((*a)->get_team() == src->get_team() ||
		         cur_dist > dist);
		ret = *a;
	}
	return ret;
}

bool Level::hazard(const Actor *actor) const
{
	Sphere sphere = actor->get_sphere();
	for (auto h : m_hazards)
		if (h->collides(&sphere))
			return true;
	return false;
}
