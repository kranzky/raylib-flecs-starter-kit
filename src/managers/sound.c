#include <raylib.h>

#include "sound.h"

//==============================================================================

static Sound _sounds[MAX_SOUNDS];

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  for (int i = 0; i < MAX_SOUNDS; ++i)
    UnloadSound(_sounds[i]);
}

//------------------------------------------------------------------------------

static void _load(int id, const char *name)
{
  char filename[256];
  sprintf(filename, "./res/sfx/%s.wav", name);
  _sounds[id] = LoadSound(filename);
}

//------------------------------------------------------------------------------

void sound_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  _load(SOUND_KRANZKY, "kranzky");
  _load(SOUND_SHOOT, "shoot");
  _load(SOUND_BOOM, "boom");
}

//------------------------------------------------------------------------------

Sound *sound_manager_get(SoundName id)
{
  return &_sounds[id];
}
