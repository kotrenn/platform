#include "hazardfactory.h"

HazardFactory *HazardFactory::m_instance = NULL;

HazardFactory *HazardFactory::get_instance()
{
	if (m_instance == NULL)
		m_instance = new HazardFactory();
	return m_instance;
}

HazardFactory::HazardFactory()
	:m_hazards()
{
	insert(new BoxHazard());
}

HazardFactory::~HazardFactory()
{
	for (auto it : m_hazards)
		delete it.second;
}

void HazardFactory::insert(Hazard *hazard)
{
	m_hazards[hazard->get_name()] = hazard;
}

Hazard *HazardFactory::load_hazard(const string &name,
                                   FILE *in,
                                   MatLib *matlib)
{
	if (m_hazards.find(name) == m_hazards.end())
	{
		fprintf(stderr, "Error: no hazard named %s\n", name.c_str());
		exit(1);
	}
	return m_hazards[name]->clone(in, matlib);
}
