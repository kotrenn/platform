#include "resourcemanager.h"

ResourceManager *ResourceManager::m_instance = NULL;

ResourceManager *ResourceManager::get_instance()
{
	if (m_instance == NULL)
		m_instance = new ResourceManager();
	return m_instance;
}

ResourceManager::ResourceManager()
	:m_images(),
	 m_chunk(),
	 m_music(),
	 m_cur_music(""),
	 m_font(NULL)
{
}

ResourceManager::~ResourceManager()
{
	delete m_font;

	for (auto si : m_images)
		delete si.second;
	for (auto ci : m_chunk)
		Mix_FreeChunk(ci.second);
	for (auto mi : m_music)
		Mix_FreeMusic(mi.second);
}

Surface *ResourceManager::get_image(const char *filename)
{
	string file_str(filename);
	if (m_images.find(file_str) == m_images.end())
	{
		char fullname[80];
		snprintf(fullname, 80, "images/%s", filename);
		m_images[file_str] = new Surface(fullname);
	}
	return m_images[file_str];
}

void ResourceManager::play_sound(const char *filename)
{
	string file_str(filename);
	if (m_chunk.find(file_str) == m_chunk.end())
	{
		char fullname[80];
		snprintf(fullname, 80, "sfx/%s", filename);
		m_chunk[file_str] = Mix_LoadWAV(fullname);
		if (m_chunk[file_str] == NULL)
			fprintf(stderr, "Mix_LoadChunk failed: %s\n",
			        SDL_GetError());
	}
	Mix_Chunk *chunk = m_chunk[file_str];
	if (Mix_PlayChannel(-1, chunk, 0) == -1)
	{
		fprintf(stderr, "Mix_PlayChannel failed: %s\n",
		        SDL_GetError());
		fprintf(stderr, "For file %s\n", filename);
	}
}

void ResourceManager::play_music(const char *filename)
{
	return;
	
	string file_str(filename);
	if (m_music.find(file_str) == m_music.end())
	{
		char fullname[80];
		snprintf(fullname, 80, "music/%s.mp3", filename);
		m_music[file_str] = Mix_LoadMUS(fullname);
		if (m_music[file_str] == NULL)
			fprintf(stderr, "Mix_LoadMUS failed: %s\n",
			        SDL_GetError());
	}
	if (m_cur_music == file_str)
		return;
	else
	{
		m_cur_music = file_str;
		Mix_PlayMusic(m_music[file_str], -1);
	}
}

Font *ResourceManager::get_font()
{
	if (m_font == NULL)
		m_font = new Font("font.png");
	return m_font;
}
