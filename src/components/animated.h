#ifndef ANIMATED_COMPONENT_H
#define ANIMATED_COMPONENT_H

#include <flecs.h>

typedef struct Animated
{
  int width;
  int height;
  int frames;
  int begin;
  double duration;
  double time;
  bool loop;
} Animated;

ECS_COMPONENT_DECLARE(Animated);

#endif
