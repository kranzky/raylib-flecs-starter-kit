#include "music.h"

//==============================================================================

static Music _music[MAX_MUSIC];

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  for (int i = 0; i < MAX_MUSIC; ++i)
    UnloadMusicStream(_music[i]);
}

//------------------------------------------------------------------------------

static void _load(int id, const char *name)
{
  char filename[256];
  sprintf(filename, "./res/mp3/%s.mp3", name);
  _music[id] = LoadMusicStream(filename);
  _music[id].looping = true;
}

//------------------------------------------------------------------------------

void music_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  _load(MUSIC_ROCK_VOMIT, "rockvomit");
}

//------------------------------------------------------------------------------

Music music_manager_get(MusicName id)
{
  return _music[id];
}
