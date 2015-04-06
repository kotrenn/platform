#include "layout.h"

Layout::Layout(Level *level,
               const vector3d &cell_dims,
               TerrainLib *terrain_lib,
               FILE *in)
	:m_level(level),
	 m_cell_dims(cell_dims),
	 m_w(level->get_w()),
	 m_h(level->get_h()),
	 m_d(level->get_d()),
	 m_terrain(NULL),
	 m_start(0, 0, 0)
{
	int vol = m_w * m_h * m_d;
	//m_terrain = (Terrain *)malloc(sizeof(Terrain) * vol);
	m_terrain = new Terrain[vol];
	for (int z = 0; z < m_d; ++z)
	{
		for (int y = m_h - 1; y >= 0; --y)
		{
			char row[m_w + 1];
			//fscanf(in, "%s", row);
			row[0] = '\0';
			while (strlen(row) == 0 || row[0] == '\n')
				fgets(row, m_w + 1, in);
			for (int x = 0; x < m_w; ++x)
			{
				char cur = row[x];
				if (cur == '@')
				{
					cur = '.';
					set_start(x, y, z);
				}
				vector3d loc = vector3d(x, y, z) ^ cell_dims;
				Box box(loc, cell_dims);
				TerrainType *type = terrain_lib->get(cur);
				*get(x, y, z) = Terrain(box, type);
				/*
				if (cur != '.')
				{
					//get(x, y, z) = m_terrain_lib->build(cur, x, y, z);
					//Box box(x, y, z, cell_dims[0], cell_dims[1], cell_dims[2]);
					vector3d loc = vector3d(x, y, z) ^ cell_dims;
					Box box(loc, cell_dims);
					*get(x, y, z) = Terrain(box, material);
				}
				else
					*get(x, y, z) = Terrain(Box(), material);
					*/
			}
		}
	}
}

Layout::~Layout()
{
	//free(m_terrain);
	delete[] m_terrain;
}

void Layout::fix(Actor *actor, Camera *camera) const
{
	vector3d center = actor->get_center();

	float cx = center.get_x();
	float cy = center.get_y();
	float cz = center.get_z();
	float r = actor->get_radius();
	int min_x = (int)((cx - r) / m_cell_dims.get_x());
	int min_y = (int)((cy - r) / m_cell_dims.get_y());
	int min_z = (int)((cz - r) / m_cell_dims.get_z());
	int max_x = (int)((cx + r) / m_cell_dims.get_x()) + 1;
	int max_y = (int)((cy + r) / m_cell_dims.get_y()) + 1;
	int max_z = (int)((cz + r) / m_cell_dims.get_z()) + 1;

	min_x = max(min_x, 0);
	min_y = max(min_y, 0);
	min_z = max(min_z, 0);

	max_x = min(max_x, m_w - 1);
	max_y = min(max_y, m_h - 1);
	max_z = min(max_z, m_d - 1);
	
	for (int z = min_z; z <= max_z; ++z)
		for (int y = min_y; y <= max_y; ++y)
			for (int x = min_x; x <= max_x; ++x)
				get(x, y, z)->fix(actor, camera);
}

bool Layout::collides(Bullet *bullet) const
{
	Shape3d *shape3d = bullet->get_shape3d();
	vector3d center = shape3d->get_center();
	
	int min_x = (int)(center.get_x() / m_cell_dims.get_x());
	int min_y = (int)(center.get_y() / m_cell_dims.get_y());
	int min_z = (int)(center.get_z() / m_cell_dims.get_z());
	int max_x = min_x + 1;
	int max_y = min_y + 1;
	int max_z = min_z + 1;

	min_x = max(min_x, 0);
	min_y = max(min_y, 0);
	min_z = max(min_z, 0);

	max_x = min(max_x, m_w - 1);
	max_y = min(max_y, m_h - 1);
	max_z = min(max_z, m_d - 1);
	
	for (int z = min_z; z <= max_z; ++z)
		for (int y = min_y; y <= max_y; ++y)
			for (int x = min_x; x <= max_x; ++x)
				if (get(x, y, z)->collides(bullet))
					return true;
	return false;
}

void Layout::draw(bool clip, int front)
{
	/*
	for (int z = 0; z < m_d; ++z)
		for (int y = 0; y < m_h; ++y)
			for (int x = 0; x < m_w; ++x)
				get(x, y, z)->draw();
	*/
	
	Camera *camera = m_level->get_camera();
	
	vector3d pos = camera->get_pos();
	int pos_x = (int)(pos.get_x() / m_cell_dims.get_x());
	int pos_y = (int)(pos.get_y() / m_cell_dims.get_y());
	int pos_z = (int)(pos.get_z() / m_cell_dims.get_z());
	
	vector3d view_x = camera->get_view_x();
	vector3d view_y = camera->get_view_y();
	vector3d view_z = view_y % view_x;

	vector3d ux(1, 0, 0);
	vector3d uy(0, 1, 0);
	vector3d uz(0, 0, 1);

	int min_x = 0;
	int min_y = 0;
	int min_z = 0;
	
	int max_x = m_w - 1;
	int max_y = m_h - 1;
	int max_z = m_d - 1;

	if (clip)
	{
		int dot_x = ux * view_z;
		int dot_y = uy * view_z;
		int dot_z = uz * view_z;

		pos_x -= front * dot_x;
		pos_y -= front * dot_y;
		pos_z -= front * dot_z;

		if (dot_x > 0)
			min_x = max(min_x, pos_x);
		if (dot_x < 0)
			max_x = min(max_x, pos_x);

		if (dot_y > 0)
			min_y = max(min_y, pos_y);
		if (dot_y < 0)
			max_y = min(max_y, pos_y);

		if (dot_z > 0)
			min_z = max(min_z, pos_z);
		if (dot_z < 0)
			max_z = min(max_z, pos_z);
	}
	
	for (int z = min_z; z <= max_z; ++z)
		for (int y = min_y; y <= max_y; ++y)
			for (int x = min_x; x <= max_x; ++x)
				get(x, y, z)->draw();
}

Terrain *Layout::get(int x, int y, int z) const
{
	return m_terrain + (x * m_h + y) * m_d + z;
}

void Layout::set_start(int x, int y, int z)
{
	printf("set_start(%d, %d, %d)\n", x, y, z);
	printf("m_cell_dims = %f %f %f\n", m_cell_dims[0], m_cell_dims[1], m_cell_dims[2]);
	m_start = m_cell_dims ^ vector3d(x, y, z);
	m_start += 0.5 * m_cell_dims;
	printf("m_start = %f %f %f\n", m_start[0], m_start[1], m_start[2]);
}
