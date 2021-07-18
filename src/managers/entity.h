#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <flecs.h>

#include "../components/scene.h"

ECS_TYPE_DECLARE(SceneType);

void entity_manager_init(ecs_world_t *world);

ecs_entity_t entity_manager_spawn_scene(SceneName id);

#endif
