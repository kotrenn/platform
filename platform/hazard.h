#ifndef __HAZARD_H
#define __HAZARD_H

#include "common.h"
#include "matlib.h"

class Hazard
{
public:
	Hazard(const string &name);
	virtual ~Hazard();

	virtual Hazard *clone(FILE *in,
	                      MatLib *matlib);

	string get_name() const { return m_name; }

	virtual void pause() {}
	virtual void unpause() {}

	virtual void update();
	virtual void move(float dt);
	virtual void draw() const;

	virtual bool collides(const Shape3d *shape3d) const = 0;
protected:
	string m_name;
};

class BoxHazard : public Hazard
{
public:
	BoxHazard();

	Hazard *clone(FILE *in,
	              MatLib *matlib);

	void draw() const;

	bool collides(const Shape3d *shape3d) const;
private:
	Material *m_material;
	Box m_dims;
};

#endif
