#ifndef NUKLEAR_MANAGER_H
#define NUKLEAR_MANAGER_H

#include <flecs.h>

#include "../components/widget.h"

void nuklear_manager_init(ecs_world_t *world);

ecs_entity_t nuklear_window(ecs_world_t *world, ecs_entity_t parent, const char *name, int x, int y, int width, int height);
ecs_entity_t nuklear_label(ecs_world_t *world, ecs_entity_t window, const char *name);
ecs_entity_t nuklear_button(ecs_world_t *world, ecs_entity_t window, const char *name, void (*callback)(Widget *widget));
ecs_entity_t nuklear_slider(ecs_world_t *world, ecs_entity_t window, float value, void (*callback)(Widget *widget));
ecs_entity_t nuklear_separator(ecs_world_t *world, ecs_entity_t window);

#endif
