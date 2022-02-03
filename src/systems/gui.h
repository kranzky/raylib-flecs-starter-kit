#ifndef GUI_SYSTEM_H
#define GUI_SYSTEM_H

#include <flecs.h>

void gui_input(ecs_iter_t *it);
void gui_update(ecs_iter_t *it);
void gui_render(ecs_iter_t *it);
void gui_reset(int row);
void update_console(ecs_iter_t *it);

#endif
