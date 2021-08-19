#ifndef TITLE_SCENE_H
#define TITLE_SCENE_H

#include <flecs.h>

#include "../components/scene.h"
#include "../components/input.h"
#include "../components/settings.h"

void init_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent);
bool update_title(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings, ecs_entity_t parent);
void fini_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent);

#endif
