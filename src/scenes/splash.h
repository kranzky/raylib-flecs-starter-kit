#ifndef SPLASH_SCENE_H
#define SPLASH_SCENE_H

#include <flecs.h>

#include "../components/scene.h"
#include "../components/input.h"
#include "../components/settings.h"

void spawn_splash(ecs_world_t *world);
void init_splash(ecs_world_t *world, ecs_entity_t parent);
bool update_splash(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings);
void fini_splash(ecs_world_t *world, const Scene *scene);

#endif
