#ifndef SCENE_SYSTEM_H
#define SCENE_SYSTEM_H

#include <flecs.h>

#include "../components/scene.h"
#include "../components/widget.h"

ecs_entity_t spawn_scene(ecs_world_t *world, SceneName id, int value);
void update_scene(ecs_iter_t *it);
void scene_volume(ecs_world_t *world, Widget *widget);

#endif
