#ifndef __TIMER_H
#define __TIMER_H

#include "common.h"

class Timer
{
public:
	Timer();
	Timer(float time);

	void pause();
	void unpause();
	void reset(float time);
	void clear();

	float get_time() const;
	float cycles_elapsed(float cycle) const;
	bool done() const { return get_time() <= 0; }
private:
	Uint32 m_start;
	Uint32 m_pause;
	float m_time;
	bool m_paused;
};

#endif
