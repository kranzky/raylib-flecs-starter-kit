#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <raylib.h>
#include <flecs.h>

typedef enum MusicName
{
  MUSIC_ROCK_VOMIT,
  MAX_MUSIC
} MusicName;

void music_manager_init(ecs_world_t *world);
Music * music_manager_get(MusicName id);

#endif
