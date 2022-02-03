#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <flecs.h>
#include <chipmunk_structs.h>

#include <flecs.h>
#include <chipmunk_structs.h>

typedef struct Physics
{
  cpSpace *space;
} Physics;

ECS_COMPONENT_DECLARE(Physics);

#endif
