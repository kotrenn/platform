#ifndef __DECORATION_H
#define __DECORATION_H

#include "surface.h"
#include "timer.h"

class Decoration
{
public:
	Decoration(FILE *in);
	~Decoration();

	void pause();
	void unpause();

	void update();
	void draw() const;
private:
	vector2d m_pos;
	int m_num_frames;
	int m_cur_frame;
	vector<Surface *> m_frames;
	float m_delay;
	Timer m_timer;
};

#endif
