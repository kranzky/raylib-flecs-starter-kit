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
Music *music_manager_get(MusicName id);
void music_manager_volume(ecs_world_t *world, ecs_entity_t id, float volume);
void music_manager_mute(ecs_world_t *world, ecs_entity_t id);
void music_manager_unmute(ecs_world_t *world, ecs_entity_t id);
void music_manager_stop(ecs_world_t *world, ecs_entity_t id);

#endif
