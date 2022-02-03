#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <flecs.h>
#include <raymath.h>

#include "../components/physics.h"

void physics_manager_init(ecs_world_t *world);
void physics_manager_set_properties(ecs_world_t *world, float gravity, float damping);
void physics_ball(ecs_world_t *world, ecs_entity_t parent, float mass, float radius, Vector2 position);
void physics_line(ecs_world_t *world, ecs_entity_t parent, Vector2 from, Vector2 to, float radius);
void physics_box(ecs_world_t *world, ecs_entity_t parent, Vector2 position);
void physics_wedge(ecs_world_t *world, ecs_entity_t parent, Vector2 position, int corner);

#ifdef DEBUG
cpSpaceDebugDrawOptions *physics_debug_options(void);
#endif

#endif
