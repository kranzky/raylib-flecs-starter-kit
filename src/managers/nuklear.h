#ifndef NUKLEAR_MANAGER_H
#define NUKLEAR_MANAGER_H

#include <flecs.h>

void nuklear_manager_init(ecs_world_t *world);

ecs_entity_t nuklear_window(ecs_world_t *world, ecs_entity_t parent, const char *name, int x, int y, int width, int height);
ecs_entity_t nuklear_label(ecs_world_t *world, ecs_entity_t window, const char *name);

#endif
