#ifndef DEBUG_SYSTEM_H
#define DEBUG_SYSTEM_H

#include <flecs.h>

void debug_input(ecs_iter_t *it);
void debug_scene(ecs_iter_t *it);
void debug_render(ecs_iter_t *it);

#endif
