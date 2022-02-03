#ifndef SPLASH_SCENE_H
#define SPLASH_SCENE_H

#include <flecs.h>

#include "../components/scene.h"
#include "../components/input.h"
#include "../components/time.h"
#include "../components/settings.h"

ecs_entity_t spawn_splash(ecs_world_t *world, int value);
void init_splash(ecs_world_t *world, ecs_entity_t parent);
bool update_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent, const Input *input, const Time *time, const Settings *settings);
void fini_splash(ecs_world_t *world, const Scene *scene);

#endif
