#include "levelreader.h"

#include "actorfactory.h"
#include "hazardfactory.h"
#include "resourcemanager.h"

void LevelReader::read(Level *level, const string &filename)
{
	fprintf(stderr, "Reading level \"%s\"\n", filename.c_str());
	
	char fullname[81];
	snprintf(fullname, 80, "levels/%s.txt", filename.c_str());

	FILE *file;
	if ((file = fopen(fullname, "r")) == NULL)
	{
		fprintf(stderr, "Could not open level %s\n", filename.c_str());
		return;
	}

	parse(level, file);
	
	fclose(file);
}

void LevelReader::parse(Level *level, FILE *in)
{
	ActorFactory *af = ActorFactory::get_instance();
	HazardFactory *hf = HazardFactory::get_instance();
	ResourceManager *rm = ResourceManager::get_instance();

	fscanf(in, "%d %d", &(level->m_w), &(level->m_h));

	//Material *cur_material = level->m_matlib->get("white");
	level->m_terrain_lib = new TerrainLib();

	char buf[81];
	while (fscanf(in, "%80s", buf) > 0)
	{
		if (strcmp(buf, "dims") == 0)
		{
			fscanf(in, "%d %d %d",
			       &(level->m_w),
			       &(level->m_h),
			       &(level->m_d));
		}
		else if (strcmp(buf, "mtllib") == 0)
		{
			char name[81];
			fscanf(in, "%80s", name);
			level->m_matlib->open(name);
		}
		else if (strcmp(buf, "background") == 0)
		{
			char bg[81];
			fscanf(in, "%80s", bg);
			level->m_background = string(bg);
		}
		else if (strcmp(buf, "cell_dims") == 0)
		{
			float w, h, d;
			fscanf(in, "%f %f %f", &w, &h, &d);
			printf("w = %.2f\th = %.2f\td = %.2f\n", w, h, d);
			level->m_cell_dims = vector3d(w, h, d);
		}
		else if (strcmp(buf, "terrain") == 0)
		{
			level->m_terrain_lib->read_terrain(level->m_matlib, in);
		}
		/*
		else if (strcmp(buf, "usemtl") == 0)
		{
			char mtl[81];
			fscanf(in, "%80s", mtl);
			cur_material = level->m_matlib->get(mtl);
		}
		*/
		else if (strcmp(buf, "axis") == 0)
		{
			char axis[81];
			float xx, xy, xz;
			float yx, yy, yz;
			fscanf(in, "%80s %f %f %f", axis, &xx, &xy, &xz);
			fscanf(in, "%80s %f %f %f", axis, &yx, &yy, &yz);
			level->m_view_x = vector3d(xx, xy, xz);
			level->m_view_y = vector3d(yx, yy, yz);
		}
		else if (strcmp(buf, "layout") == 0)
		{
			level->m_layout = new Layout(level,
			                             level->m_cell_dims,
			                             level->m_terrain_lib,
			                             in);
		}
		else if (strcmp(buf, "music") == 0)
		{
			char song[81];
			fscanf(in, "%80s", song);
			rm->play_music(song);
		}
		else if (strcmp(buf, "enable") == 0)
		{
			char option[81];
			fscanf(in, "%80s", option);
			level->set_option(option, true);
		}
		else if (strcmp(buf, "front") == 0)
		{
			fscanf(in, "%d", &(level->m_front));
		}
		else if (strcmp(buf, "actor") == 0)
		{
			char name[81];
			fscanf(in, "%80s", name);
			Actor *actor = af->load_actor(level, name, in);
			level->add_actor(actor);
		}
		else if (strcmp(buf, "goal") == 0)
		{
			Goal *goal = new Goal(in);
			level->m_goals.push_back(goal);
		}
		else if (strcmp(buf, "decoration") == 0)
		{
			Decoration *deco = new Decoration(in);
			level->m_decorations.push_back(deco);
		}
		else if (strcmp(buf, "hazard") == 0)
		{
			char name[81];
			fscanf(in, "%80s", name);
			Hazard *hazard = hf->load_hazard(name, in, level->m_matlib);
			level->m_hazards.push_back(hazard);
		}
		else
		{
			fprintf(stderr, "Unknown level command: %s\n", buf);
			fflush(stderr);
		}
	}
}	
