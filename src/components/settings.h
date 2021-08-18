#ifndef SETTINGS_COMPONENT_H
#define SETTINGS_COMPONENT_H

#include <flecs.h>

typedef struct Settings
{
  bool music;
  bool fullscreen;
  int gamepad;
} Settings;

ECS_COMPONENT_DECLARE(Settings);

#endif
