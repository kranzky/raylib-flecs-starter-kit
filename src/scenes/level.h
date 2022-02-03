#ifndef LEVEL_SCENE_H
#define LEVEL_SCENE_H

#include <flecs.h>

#include "../components/scene.h"
#include "../components/input.h"
#include "../components/settings.h"
#include "../components/time.h"

ecs_entity_t spawn_level(ecs_world_t *world, int value);
void init_level(ecs_world_t *world, ecs_entity_t parent);
bool update_level(ecs_world_t *world, const Scene *scene, ecs_entity_t parent, const Input *input, const Time *time, const Settings *settings);
void fini_level(ecs_world_t *world, const Scene *scene);

#endif
