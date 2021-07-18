#include <raylib.h>
#include <raymath.h>
#include <flecs.h>

#include "../defines.h"

#include "scene.h"

#include "../scenes/splash.h"
#include "../scenes/title.h"

//==============================================================================

// TODO: jump table

//==============================================================================

static inline void _init(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  switch (scene->id)
  {
  case SCENE_SPLASH:
    init_splash(world, scene, entity);
    break;
  case SCENE_TITLE:
    init_title(world, scene, entity);
    break;
  default:
    break;
  }
  scene->state = SCENE_STATE_RUNNING;
}

//------------------------------------------------------------------------------

static inline void _update(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  bool status = false;
  switch (scene->id)
  {
  case SCENE_SPLASH:
    status = update_splash(world, scene, entity);
    break;
  case SCENE_TITLE:
    status = update_title(world, scene, entity);
    break;
  default:
    break;
  }
  if (!status)
    scene->state = SCENE_STATE_STOPPING;
}

//------------------------------------------------------------------------------

static inline void _fini(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  switch (scene->id)
  {
  case SCENE_SPLASH:
    fini_splash(world, scene, entity);
    break;
  case SCENE_TITLE:
    fini_title(world, scene, entity);
    break;
  default:
    break;
  }
  ecs_delete(world, entity);
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
      _init(it->world, &scene[i], it->entities[i]);
      break;
    case SCENE_STATE_RUNNING:
      _update(it->world, &scene[i], it->entities[i]);
      scene[i].time += it->delta_time;
      break;
    case SCENE_STATE_STOPPING:
      _fini(it->world, &scene[i], it->entities[i]);
      break;
    default:
      TraceLog(LOG_WARNING, "bad scene state");
      break;
    }
  }
}
