#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <flecs.h>

void system_manager_init(ecs_world_t *world);
ecs_query_t *system_manager_viewport_query(void);

#endif
