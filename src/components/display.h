#ifndef DISPLAY_COMPONENT_H
#define DISPLAY_COMPONENT_H

#include <raylib.h>
#include <raymath.h>
#include <flecs.h>

typedef struct Display
{
  float scale;
  Color border;
  Color background;
  Rectangle raster;
  Rectangle window;
  Rectangle screen;
} Display;

ECS_COMPONENT_DECLARE(Display);

#endif
