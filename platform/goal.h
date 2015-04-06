#ifndef __GOAL_H
#define __GOAL_H

#include "common.h"

class Goal
{
public:
	Goal(FILE *in);

	bool collides(const Shape3d *shape3d) const;
	string get_next() const { return m_next; }
	vector3d get_axis() const { return m_axis; }

	void draw() const;
private:
	string m_next;
	Box m_dims;
	vector3d m_axis;
};

#endif
