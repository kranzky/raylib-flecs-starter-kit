#ifndef STATEFUL_COMPONENT_H
#define STATEFUL_COMPONENT_H

#include <flecs.h>

typedef enum StateName
{
  STATE_CREATED,
  STATE_STARTING,
  STATE_RUNNING,
  STATE_STOPPING,
  STATE_STOPPED,
  MAX_STATES
} StateName;

typedef struct Stateful
{
  StateName state;
  float timer;
  float start_time;
  float run_time;
  float stop_time;
  bool transitioned;
} Stateful;

ECS_COMPONENT_DECLARE(Stateful);

#endif
