#ifndef __LEVEL_READER_H
#define __LEVEL_READER_H

#include "level.h"

class Level;

class LevelReader
{
public:
	static void read(Level *level, const string &filename);
private:
	static void parse(Level *level, FILE *in);
};

#endif
