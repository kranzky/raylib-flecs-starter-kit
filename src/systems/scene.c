#include <raylib.h>
#include <raymath.h>

#include "../defines.h"

#include "../components/input.h"
#include "../components/settings.h"

#include "../scenes/splash.h"
#include "../scenes/title.h"
#include "../scenes/level.h"

#include "scene.h"

//==============================================================================

typedef void (*voidSceneFunction)(ecs_world_t *world, const Scene *scene, ecs_entity_t entity);
typedef bool (*boolSceneFunction)(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings, ecs_entity_t entity);

typedef struct JumpTarget
{
  voidSceneFunction init;
  boolSceneFunction update;
  voidSceneFunction fini;
} JumpTarget;

JumpTarget _targets[] = {[SCENE_TITLE] = {.init = init_title, .update = update_title, .fini = fini_title},
                         [SCENE_SPLASH] = {.init = init_splash, .update = update_splash, .fini = fini_splash},
                         [SCENE_LEVEL] = {.init = init_level, .update = update_level, .fini = fini_level}};

//==============================================================================

static inline void _init(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  _targets[scene->id].init(world, scene, entity);
  scene->state = SCENE_STATE_RUNNING;
}

//------------------------------------------------------------------------------

static inline void _update(ecs_world_t *world, Scene *scene, const Input *input, const Settings *settings, ecs_entity_t entity)
{
  if (!_targets[scene->id].update(world, scene, input, settings, entity))
    scene->state = SCENE_STATE_STOPPING;
}

//------------------------------------------------------------------------------

static inline void _fini(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  _targets[scene->id].fini(world, scene, entity);
  ecs_delete(world, entity);
}

//==============================================================================

void update_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_column(it, Scene, 1);
  Input *input = ecs_column(it, Input, 2);
  Settings *settings = ecs_column(it, Settings, 3);
  for (int i = 0; i < it->count; ++i)
  {
    switch (scene[i].state)
    {
    case SCENE_STATE_STARTING:
      _init(it->world, &scene[i], it->entities[i]);
      break;
    case SCENE_STATE_RUNNING:
      _update(it->world, &scene[i], input, settings, it->entities[i]);
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
