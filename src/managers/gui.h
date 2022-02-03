#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <flecs.h>

#include "../components/widget.h"

void gui_manager_init(ecs_world_t *world);

ecs_entity_t gui_window(ecs_world_t *world, ecs_entity_t parent, const char *name, int x, int y, int width, int height, int row, int max);
ecs_entity_t gui_label(ecs_world_t *world, ecs_entity_t window, const char *name);
ecs_entity_t gui_button(ecs_world_t *world, ecs_entity_t window, const char *name, int value, widgetCallback callback);
ecs_entity_t gui_slider(ecs_world_t *world, ecs_entity_t window, float value, widgetCallback callback);
ecs_entity_t gui_separator(ecs_world_t *world, ecs_entity_t window);

#endif
