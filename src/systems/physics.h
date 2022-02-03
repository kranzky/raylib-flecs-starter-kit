#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <flecs.h>

void physics_update(ecs_iter_t *it);
void physics_collide(ecs_iter_t *it);

#endif
