#ifndef __LEVEL_SELECTOR_H
#define __LEVEL_SELECTOR_H

#include "camera.h"
#include "level.h"
#include "player.h"

class LevelSelector
{
public:
	LevelSelector();
	~LevelSelector();

	void pause();
	void unpause();

	void set_player(PlayerActor *player);
	PlayerActor *get_player() const { return m_player; }

	void update();
	void move(float dt);
	void draw(Surface *dst) const;

	Level *get_level() const { return m_level; }

	void switch_level(const string &next);
private:
	Level *m_level;
	PlayerActor *m_player;
	bool m_just_switched;
};

#endif
