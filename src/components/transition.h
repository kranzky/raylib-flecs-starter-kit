#ifndef TRANSITION_COMPONENT_H
#define TRANSITION_COMPONENT_H

#include <raylib.h>
#include <flecs.h>

typedef enum TransitionName
{
  TRANSITION_FADE_IN,
  TRANSITION_DISPLAY,
  TRANSITION_FADE_OUT,
  TRANSITION_DESTROY,
  MAX_TRANSITIONS
} TransitionName;

typedef struct Transition
{
  TransitionName id;
  Color tint;
  float fade;
  float show;
  float time;
} Transition;

ECS_COMPONENT_DECLARE(Transition);

#endif
