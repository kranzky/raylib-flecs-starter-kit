#ifndef TIME_COMPONENT_H
#define TIME_COMPONENT_H

#include <flecs.h>

typedef struct Time
{
  bool paused;
  float total;
  float delta;
  float scale;
} Time;

ECS_COMPONENT_DECLARE(Time);

#endif
