#ifndef WINDOW_COMPONENT_H
#define WINDOW_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

// TODO: call nk_window_close on destruction; use flecs callback

typedef struct Window
{
  const char *name;
  Rectangle bounds;
  unsigned int flags;
} Window;

ECS_COMPONENT_DECLARE(Window);

#endif
