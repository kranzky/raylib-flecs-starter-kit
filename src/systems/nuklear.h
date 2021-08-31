#ifndef NUKLEAR_SYSTEM_H
#define NUKLEAR_SYSTEM_H

#include <flecs.h>

void nuklear_input(ecs_iter_t *it);
void nuklear_update(ecs_iter_t *it);
void nuklear_render(ecs_iter_t *it);

#endif
