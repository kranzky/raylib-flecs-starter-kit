#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include <raymath.h>
#include <flecs.h>

typedef struct Input
{
  bool quit;
  bool select;
  bool toggle_pause;
  bool toggle_debug;
  bool toggle_fullscreen;
  Vector2 joystick;
  Vector2 pointer;
  float wheel;
  bool fire;
} Input;

ECS_COMPONENT_DECLARE(Input);

#endif
