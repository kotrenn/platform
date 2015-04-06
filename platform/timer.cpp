#include "timer.h"

Timer::Timer()
	:m_start(SDL_GetTicks()),
	 m_pause(0),
	 m_time(0),
	 m_paused(false)
{
}

Timer::Timer(float time)
	:m_start(SDL_GetTicks()),
	 m_pause(0),
	 m_time(time),
	 m_paused(false)
{
}

void Timer::pause()
{
	if (!m_paused)
	{
		m_paused = true;
		m_pause = SDL_GetTicks() - m_start;
	}
}

void Timer::unpause()
{
	if (m_paused)
	{
		m_paused = false;
		m_start = SDL_GetTicks() - m_pause;
		m_pause = 0;
	}
}

void Timer::reset(float time)
{
	m_start = SDL_GetTicks();
	m_pause = 0;
	m_time = time;
	m_paused = false;
}

void Timer::clear()
{
	m_time = -1;
}

float Timer::get_time() const
{
	float ret;
	if (m_paused)
		ret = m_pause / 1000.0;
	else
		ret = (SDL_GetTicks() - m_start) / 1000.0;
	return m_time - ret;
}

float Timer::cycles_elapsed(float cycle) const
{
	float ret;
	if (m_paused)
		ret = m_pause / 1000.0;
	else
		ret = (SDL_GetTicks() - m_start) / 1000.0;
	return ret / cycle;
}
