#ifndef SPLASH_SCENE_H
#define SPLASH_SCENE_H

#include <flecs.h>

#include "../components/scene.h"

void init_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent);
bool update_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent);
void fini_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent);

#endif
