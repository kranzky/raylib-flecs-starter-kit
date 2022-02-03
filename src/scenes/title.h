#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include <flecs.h>

#include "../components/scene.h"
#include "../components/input.h"
#include "../components/time.h"
#include "../components/settings.h"

ecs_entity_t spawn_title(ecs_world_t *world, int value);
void init_title(ecs_world_t *world, ecs_entity_t parent);
bool update_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent, const Input *input, const Time *time, const Settings *settings);
void fini_title(ecs_world_t *world, const Scene *scene);

#endif
