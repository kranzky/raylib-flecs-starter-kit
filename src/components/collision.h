#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include <flecs.h>
#include <raymath.h>

#include "physical.h"

typedef struct Collision
{
  ecs_entity_t entities[2];
  Vector2 contacts[2];
  Vector2 normal;
  float energy;
} Collision;

ECS_COMPONENT_DECLARE(Collision);

#endif
