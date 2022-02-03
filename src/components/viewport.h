#ifndef VIEWPORT_COMPONENT_H
#define VIEWPORT_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

typedef struct Viewport
{
  RenderTexture2D raster;
  Vector2 size;
  Color background;
  Camera2D camera;
  Rectangle src;
  Rectangle dst;
  Vector2 origin;
  float rotation;
  Color color;
  Vector2 pointer;
  Vector2 handle;
  bool active;
} Viewport;

ECS_COMPONENT_DECLARE(Viewport);

#endif
