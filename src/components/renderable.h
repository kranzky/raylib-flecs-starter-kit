#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

typedef struct Renderable
{
  const Texture *texture;
  BlendMode blend_mode;
  Rectangle src;
  float scale;
} Renderable;

ECS_COMPONENT_DECLARE(Renderable);

#endif
