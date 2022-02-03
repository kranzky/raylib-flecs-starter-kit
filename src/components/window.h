#ifndef WINDOW_COMPONENT_H
#define WINDOW_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

typedef struct Window
{
  const char *name;
  Rectangle bounds;
  unsigned int flags;
  unsigned int max;
} Window;

ECS_COMPONENT_DECLARE(Window);

#endif
