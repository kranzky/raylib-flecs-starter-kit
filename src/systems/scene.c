#include <raylib.h>
#include <raymath.h>
#include <flecs.h>

#include "../defines.h"

#include "scene.h"

#include "../scenes/splash.h"

//==============================================================================

// TODO: jump table

//==============================================================================

static inline void _init(ecs_world_t *world, const Scene *scene)
{
  switch (scene->id)
  {
  case SCENE_SPLASH:
    init_splash(world, scene);
    break;
  default:
    break;
  }
}

//------------------------------------------------------------------------------

static inline bool _update(ecs_world_t *world, const Scene *scene)
{
  switch (scene->id)
  {
  case SCENE_SPLASH:
    return update_splash(world, scene);
  default:
    break;
  }
  return false;
}

//------------------------------------------------------------------------------

static inline void _fini(ecs_world_t *world, const Scene *scene)
{
  switch (scene->id)
  {
  case SCENE_SPLASH:
    fini_splash(world, scene);
    break;
  default:
    break;
  }
}

//==============================================================================

void update_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_column(it, Scene, 1);
  for (int i = 0; i < it->count; ++i)
  {
    switch (scene[i].state)
    {
    case SCENE_STATE_STARTING:
      _init(it->world, &scene[i]);
      scene[i].state = SCENE_STATE_RUNNING;
      break;
    case SCENE_STATE_RUNNING:
      if (!_update(it->world, &scene[i]))
        scene[i].state = SCENE_STATE_STOPPING;
      else
        scene[i].time += it->delta_time;
      break;
    case SCENE_STATE_STOPPING:
      _fini(it->world, &scene[i]);
      ecs_delete(it->world, it->entities[i]);
      break;
    default:
      TraceLog(LOG_WARNING, "bad scene state");
      break;
    }
  }
}
