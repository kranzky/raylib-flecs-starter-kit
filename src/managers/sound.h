#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <flecs.h>

typedef enum SoundName
{
  SOUND_KRANZKY,
  SOUND_SHOOT,
  SOUND_BOOM,
  MAX_SOUNDS
} SoundName;

void sound_manager_init(ecs_world_t *world);
void sound_manager_play(SoundName id, float volume);

#endif
