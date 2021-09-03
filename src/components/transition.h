#ifndef TRANSITION_COMPONENT_H
#define TRANSITION_COMPONENT_H

#include <raylib.h>
#include <flecs.h>

typedef enum TransitionName
{
  TRANSITION_FADE_IN,
  TRANSITION_FADE_OUT,
  MAX_TRANSITIONS
} TransitionName;

typedef struct Transition
{
  TransitionName id;
  float fade;
  float time;
} Transition;

ECS_COMPONENT_DECLARE(Transition);

#endif
