#ifndef SPATIAL_COMPONENT_H
#define SPATIAL_COMPONENT_H

#include <flecs.h>
#include <raymath.h>

typedef struct Spatial
{
  Vector2 position;
  float rotation;
} Spatial;

ECS_COMPONENT_DECLARE(Spatial);

#endif
