#include "player.h"
#include "datamanager.h"
#include "resourcemanager.h"

PlayerActor::PlayerActor(Level *level, InputDevice *device)
	:Actor(level, "player", 4, 0.2),
	 InputListener(device),
	 m_fire_rate(0),
	 m_fire_timer(0),
	 m_select_timer(0),
	 m_jump_timer(0),
	 m_health_timer(0)
{
	m_team = TEAM_PLAYER;

	reset();
}

PlayerActor::~PlayerActor()
{
}

void PlayerActor::pause()
{
	Actor::pause();

	m_fire_timer.pause();
	m_select_timer.pause();
	m_jump_timer.pause();
	m_health_timer.pause();
}

void PlayerActor::unpause()
{
	Actor::unpause();

	m_fire_timer.unpause();
	m_select_timer.unpause();
	m_jump_timer.unpause();
	m_health_timer.unpause();
}

void PlayerActor::reset()
{
	DataManager *dm = DataManager::get_instance();

	m_fire_rate = dm->get_flt("pbullet_delay");
	reset_health(dm->get_int("player_health"));
	m_dead = false;
	m_invincible = false;
}

void PlayerActor::button_pressed(Button button)
{
	Camera *camera = m_level->get_camera();
	if (camera->is_moving())
		return;

	vector3d view_x = camera->get_view_x();
	vector3d view_y = camera->get_view_y();
	vector3d view_z = view_x % view_y;

	vector3d axis(0, 0, 0);
	vector3d init(0, 0, 0);
	
	if (button == BUTTON_L_SPACE)
	{
		axis = view_y;
		init = view_x;
		camera->rotate_space(-1);
	}
	else if (button == BUTTON_R_SPACE)
	{
		axis = -1.0 * view_y;
		init = view_x;
		camera->rotate_space(1);
	}
	else if (button == BUTTON_L_GRAV)
	{
		axis = view_z;
		init = view_x;
		camera->rotate_gravity(-1);
	}
	else if (button == BUTTON_R_GRAV)
	{
		axis = -1.0 * view_z;
		init = view_x;
		camera->rotate_gravity(1);
	}

	//return;
	DataManager *dm = DataManager::get_instance();
	int special_power = dm->get_int("special_power");
	if (!special_power)
		return;

	if (axis * axis < EPSILON ||
	    init * init < EPSILON)
		return;

	int special_count = dm->get_int("special_count");
	for (int i = 0; i < special_count; ++i)
	{
		float theta = randint(1, 360);
		float radius = 3 * 0.2 * randint(2, 8);
		vector3d v = radius * init;
		v = axis.rotate(theta, v);
		Sphere pos = m_sphere;
		pos += v;
		v = init % axis;
		float speed = 10 * 0.2 * randint(2, 8);
		v *= speed;
		v = axis.rotate(theta, v);
		pos[3] = 0.05 * randint(3, 8);
		float time = 0.05 * randint(10, 40);
		time += 2.0;
		//new Particle(m_level, mapRGB(0, 0, 255), pos, v, time);
		new Bullet(this, new SphereBullet(pos, v));
	}
}

void PlayerActor::damage()
{
	Actor::damage();

	DataManager *dm = DataManager::get_instance();
	m_health_timer.reset(dm->get_flt("heal_delay"));
}

void PlayerActor::update()
{
	Actor::update();

	m_v[0] = 0;
	m_v[1] = 0;
	m_v[2] = 0;

	DataManager *dm = DataManager::get_instance();

	float speed = dm->get_flt("player_speed");

	Camera *camera = m_level->get_camera();

	/*
	if (m_input->pressed(BUTTON_UP))
		m_v += speed * camera->get_view_y();
	if (m_input->pressed(BUTTON_DOWN))
		m_v -= speed * camera->get_view_y();
	*/
	if (m_input->pressed(BUTTON_LEFT))
		m_v -= speed * camera->get_view_x();
	if (m_input->pressed(BUTTON_RIGHT))
		m_v += speed * camera->get_view_x();

	// jump
	if (m_input->pressed(BUTTON_JUMP) && !m_jumpable.done())
	{
		if (m_jump.done())
		{
			float jump_delay = dm->get_flt("jump_delay");
			m_jump_timer.reset(jump_delay);
			float jump_time = dm->get_flt("jump_time");
			m_jump.reset(jump_time);
			m_grav = vector3d(0, 0, 0);
			printf("begin jumping!\n");
		}
	}
	else
		m_jump.clear();
	if (m_jump_timer.done())
		m_jump.clear();

	// shoot
	if (m_input->pressed(BUTTON_SHOOT))
	{
		if (m_fire_timer.done())
		{
			m_fire_timer.reset(m_fire_rate);
			create_bullet();
		}
	}

	// select
	if (m_input->pressed(BUTTON_SELECT))
	{
		if (m_select_timer.done())
		{
			select_target();
			if (m_target != NULL)
				m_select_timer.reset(0.33);
		}
	}

	// heal
	if (m_health_timer.done() && m_health < m_health_max)
	{
		m_health++;
		m_health_timer.reset(dm->get_flt("heal_delay"));
	}
}

void PlayerActor::draw() const
{
	Actor::draw();

	return;

	// axes
	Camera *camera = m_level->get_camera();
	vector3d center = get_center();
	vector3d x = camera->get_view_x();
	vector3d y = camera->get_view_y();
	x = 1.5 * m_sphere.get_r() * x / x.norm();
	y = 1.5 * m_sphere.get_r() * y / y.norm();
	glBegin(GL_LINES);

	glVertex3f(center[0], center[1], center[2]);
	glVertex3f(center[0] + x[0], center[1] + x[1], center[2] + x[2]);
	glVertex3f(center[0], center[1], center[2]);
	glVertex3f(center[0] + y[0], center[1] + y[1], center[2] + y[2]);
	
	glEnd();

	// target
	if (m_target == NULL)
		return;

	Sphere sphere = m_target->get_sphere();
	float r = sphere[3] * 1.5;

	GLfloat mat_color[] = { 1.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_color);
	
	glBegin(GL_LINES);
	glVertex3f(sphere[0] - r, sphere[1], sphere[2]);
	glVertex3f(sphere[0] + r, sphere[1], sphere[2]);
	glVertex3f(sphere[0], sphere[1] - r, sphere[2]);
	glVertex3f(sphere[0], sphere[1] + r, sphere[2]);
	glVertex3f(sphere[0], sphere[1], sphere[2] - r);
	glVertex3f(sphere[0], sphere[1], sphere[2] + r);
	glEnd();
}

void PlayerActor::create_bullet()
{
	DataManager *dm = DataManager::get_instance();
	vector2d m_center = get_center();

	Camera *camera = m_level->get_camera();
	vector3d view_x = camera->get_view_x();
	vector3d view_y = camera->get_view_y();
	vector3d dir(0, 0, 0);

	if (m_input->pressed(BUTTON_UP))
		dir = view_y;
	if (m_input->pressed(BUTTON_DOWN))
		dir = -1 * view_y;
	if (m_input->pressed(BUTTON_LEFT))
		dir = -1 * view_x;
	if (m_input->pressed(BUTTON_RIGHT))
		dir = view_x;
	if (dir * dir < EPSILON)
	{
		dir = m_prev_v;
		if (dir * dir < EPSILON)
			dir = view_x;
		else
		{
			dir.normalize();
			dir = (dir * view_x) * view_x;
			if (dir * dir < EPSILON)
				dir = view_x;
		}
	}

	float x = m_sphere[0];
	float y = m_sphere[1];
	float z = m_sphere[2];
	Sphere sphere(x, y, z, 0.4 * m_sphere.get_r());
	float s = dm->get_flt("pbullet_speed");
	vector3d v = s * dir;
	new Bullet(this, new SphereBullet(sphere, v));

	ResourceManager *rm = ResourceManager::get_instance();
	rm->play_sound("player.wav");
}
