#ifndef SPLASH_SCENE_H
#define SPLASH_SCENE_H

#include <flecs.h>

#include "../components/scene.h"

void init_splash(ecs_world_t *world, const Scene *scene);
bool update_splash(ecs_world_t *world, const Scene *scene);
void fini_splash(ecs_world_t *world, const Scene *scene);

#endif
