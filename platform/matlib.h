#ifndef __MAT_LIB_H
#define __MAT_LIB_H

#include "material.h"

class MatLib
{
public:
	MatLib();
	~MatLib();

	void open(const string &filename);
	Material *get(const string &name);
private:
	void corrupt(const string &filename);
	int whitespace(FILE *in);
	int readc(FILE *in, const string &filename);
	int get_line(FILE *in);
	void get_mtl(FILE *in, const string &filename);
	void get_att(FILE *in, const string &filename, const string &att);
	void get_shininess(FILE *in, const string &filename);

	map<string, Material *> m_materials;
	string m_cur_name;
	Material *m_cur;
	set<string> m_files;
};

#endif
