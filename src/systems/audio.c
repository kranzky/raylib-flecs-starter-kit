#include <raylib.h>

#include "../components/audible.h"

//==============================================================================

void play_sounds(ecs_iter_t *it)
{
  Audible *audible = ecs_column(it, Audible, 1);
  for (int i = 0; i < it->count; ++i)
  {
    if (IsAudioDeviceReady())
    {
      SetSoundVolume(*audible[i].sound, audible[i].volume);
      PlaySound(*audible[i].sound);
    }
    ecs_delete(it->world, it->entities[i]);
  }
}
