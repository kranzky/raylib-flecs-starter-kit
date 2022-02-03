#include <raylib.h>
#include <raymath.h>

#include "../defines.h"

#include "../components/input.h"
#include "../components/time.h"
#include "../components/transition.h"
#include "../components/stateful.h"
#include "../components/tinted.h"

#include "time.h"

//==============================================================================

void progress_time(ecs_iter_t *it)
{
  Time *time = ecs_term(it, Time, 1);
  time->scale = time->paused ? 0 : 1;
  time->delta = it->delta_time * time->scale;
}

//------------------------------------------------------------------------------

void transition(ecs_iter_t *it)
{
  Time *time = ecs_term(it, Time, 1);
  Transition *transition = ecs_term(it, Transition, 2);
  for (int i = 0; i < it->count; ++i)
  {
    if (transition[i].complete)
      continue;
    switch (transition[i].id)
    {
    case TRANSITION_WAITING:
      transition[i].fade = 0;
      transition[i].wait -= time->delta;
      break;
    case TRANSITION_FADE_IN:
      transition[i].fade = Clamp(transition[i].time * 4, 0, 1);
      break;
    case TRANSITION_HOLDING:
      transition[i].fade = 1;
      transition[i].hold -= time->delta;
      break;
    case TRANSITION_FADE_OUT:
      transition[i].fade = Clamp(1 - transition[i].time * 4, 0, 1);
      break;
    };
    transition[i].time += time->delta;
    if (transition[i].time > 0.25)
    {
      transition[i].time = 0.25;
      switch (transition[i].id)
      {
      case TRANSITION_WAITING:
        if (transition[i].wait < 0)
        {
          transition[i].id = TRANSITION_FADE_IN;
          transition[i].time = 0;
        }
        break;
      case TRANSITION_FADE_IN:
        if (transition[i].hold > 0)
          transition[i].id = TRANSITION_HOLDING;
        else
        {
          transition[i].complete = true;
          transition[i].fade = 1;
        }
        break;
      case TRANSITION_HOLDING:
        if (transition[i].hold < 0)
        {
          transition[i].id = TRANSITION_FADE_OUT;
          transition[i].time = 0;
        }
        break;
      case TRANSITION_FADE_OUT:
        transition[i].complete = true;
        transition[i].fade = 0;
        break;
      };
    }
  }
}

//------------------------------------------------------------------------------

void state_machine(ecs_iter_t *it)
{
  Time *time = ecs_term(it, Time, 1);
  Stateful *stateful = ecs_term(it, Stateful, 2);
  for (int i = 0; i < it->count; ++i)
  {
    stateful[i].transitioned = (stateful[i].timer <= 0);
    stateful[i].timer += time->delta;
    if (stateful[i].timer <= 0)
      continue;
    switch (stateful[i].state)
    {
    case STATE_CREATED:
    {
      if (!stateful[i].transitioned)
      {
        stateful[i].state = STATE_STARTING;
        stateful[i].timer = 0;
      }
      break;
    }
    case STATE_STARTING:
    {
      if (stateful[i].timer >= stateful[i].start_time)
      {
        stateful[i].state = STATE_RUNNING;
        stateful[i].timer = 0;
      }
      break;
    }
    case STATE_RUNNING:
    {
      if (stateful[i].timer >= stateful[i].run_time)
      {
        stateful[i].state = STATE_STOPPING;
        stateful[i].timer = 0;
      }
      break;
    }
    case STATE_STOPPING:
    {
      if (stateful[i].timer >= stateful[i].stop_time)
      {
        stateful[i].state = STATE_STOPPED;
        stateful[i].timer = 0;
      }
      break;
    }
    case STATE_STOPPED:
    {
      ecs_delete(it->world, it->entities[i]);
      break;
    }
    }
  }
}

