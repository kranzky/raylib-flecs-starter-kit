#ifndef SETTINGS_COMPONENT_H
#define SETTINGS_COMPONENT_H

#include <raymath.h>
#include <flecs.h>

typedef struct Settings
{
  bool music;
  bool fullscreen;
  int gamepad;
  Rectangle bounds;
} Settings;

ECS_COMPONENT_DECLARE(Settings);

#endif
