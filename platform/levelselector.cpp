#include "levelselector.h"

#include "datamanager.h"
#include "resourcemanager.h"
#include "system.h"

LevelSelector::LevelSelector()
	:m_level(NULL),
	 m_player(NULL),
	 m_just_switched(true)
{
	DataManager *dm = DataManager::get_instance();

	System *system = System::get_instance();

	int system_w;
	int system_h;
	system->get_size(&system_w, &system_h);

	char level_name[81];
	dm->get_str("start_level", level_name);
	m_level = new Level(level_name);
}

LevelSelector::~LevelSelector()
{
	delete m_level;
}

void LevelSelector::pause()
{
	m_level->pause();
}

void LevelSelector::unpause()
{
	m_level->unpause();
}

void LevelSelector::set_player(PlayerActor *player)
{
	m_player = player;
	m_player->set_center(m_level->get_start());
	//m_level->add_actor(m_player);
	m_player->set_level(m_level);

	Camera *camera = m_level->get_camera();
	camera->set_target(m_player);
	camera->set_absolute(m_level->get_start());
}

void LevelSelector::update()
{
	if (m_just_switched)
		return;
	
	m_level->update();

	if (m_level->hazard(m_player))
		m_player->damage();
}

void LevelSelector::move(float dt)
{
	if (m_just_switched)
	{
		m_just_switched = false;
		return;
	}

	dt = min(dt, 0.5f);
	
	m_level->move(dt);

	/* Check if level is done; if so move to next */
	string next;
	if (m_level->goal(m_player, &next))
		switch_level(next);
}

void LevelSelector::draw(Surface *dst) const
{
	m_level->draw();
}

void LevelSelector::switch_level(const string &next)
{
	fprintf(stderr, "switch_level(%s)\n", next.c_str());

	DataManager *dm = DataManager::get_instance();

	/* Build the level and copy bullets over */
	delete m_level;
	m_level = new Level(next);

	/* Move player to new level */
	set_player(m_player);
	m_player->set_v(vector3d(0, 0, 0));
	m_player->reset_grav();
	m_player->clear_jump();
	
	/* Play beginning sound */
	char start[81];
	dm->get_str("start_level", start);
	if (next == start || true)
	{
		ResourceManager *rm = ResourceManager::get_instance();
		rm->play_sound("wtf.wav");
	}

	m_just_switched = false;
}
