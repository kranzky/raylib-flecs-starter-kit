#ifndef SCENE_SYSTEM_H
#define SCENE_SYSTEM_H

#include <flecs.h>

#include "../components/scene.h"

void spawn_scene(ecs_world_t *world, SceneName id);
void update_scene(ecs_iter_t *it);

#endif
