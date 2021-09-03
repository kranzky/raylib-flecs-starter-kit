#include <raylib.h>
#include <raymath.h>

#include "../defines.h"

#include "../components/input.h"
#include "../components/settings.h"

#include "../managers/entity.h"

#include "../scenes/splash.h"
#include "../scenes/title.h"
#include "../scenes/level.h"

#include "scene.h"

//==============================================================================

typedef void (*spawnScene)(ecs_world_t *world);
typedef void (*initScene)(ecs_world_t *world, ecs_entity_t entity);
typedef bool (*updateScene)(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings);
typedef void (*finiScene)(ecs_world_t *world, const Scene *scene);

typedef struct JumpTarget
{
  spawnScene spawn;
  initScene init;
  updateScene update;
  finiScene fini;
} JumpTarget;

JumpTarget _targets[] = {[SCENE_TITLE] = {.spawn = spawn_title, .init = init_title, .update = update_title, .fini = fini_title},
                         [SCENE_SPLASH] = {.spawn = spawn_splash, .init = init_splash, .update = update_splash, .fini = fini_splash},
                         [SCENE_LEVEL] = {.spawn = spawn_level, .init = init_level, .update = update_level, .fini = fini_level}};

//==============================================================================

static inline void _init(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  _targets[scene->id].init(world, entity);
  entity_manager_spawn_transition(world, TRANSITION_FADE_IN);
  scene->state = SCENE_STATE_RUNNING;
  scene->time = 0;
}

//------------------------------------------------------------------------------

static inline void _update(ecs_world_t *world, Scene *scene, const Input *input, const Settings *settings)
{
  if (scene->time > 0.3 && !_targets[scene->id].update(world, scene, input, settings))
  {
    scene->state = SCENE_STATE_STOPPING;
    scene->time = 0;
    entity_manager_spawn_transition(world, TRANSITION_FADE_OUT);
  }
}

//------------------------------------------------------------------------------

static inline void _fini(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  if (scene->time > 0.3)
  {
    _targets[scene->id].fini(world, scene);
    ecs_delete(world, entity);
  }
}

//==============================================================================

void spawn_scene(ecs_world_t *world, SceneName id)
{
  _targets[id].spawn(world);
}

//------------------------------------------------------------------------------

void update_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_column(it, Scene, 1);
  Input *input = ecs_column(it, Input, 2);
  Settings *settings = ecs_column(it, Settings, 3);
  if (input->toggle_fullscreen)
    ToggleFullscreen();
  for (int i = 0; i < it->count; ++i)
  {
    switch (scene[i].state)
    {
    case SCENE_STATE_STARTING:
      _init(it->world, &scene[i], it->entities[i]);
      break;
    case SCENE_STATE_RUNNING:
      _update(it->world, &scene[i], input, settings);
      break;
    case SCENE_STATE_STOPPING:
      _fini(it->world, &scene[i], it->entities[i]);
      break;
    default:
      TraceLog(LOG_WARNING, "bad scene state");
      break;
    };
    scene[i].time += it->delta_time;
  }
}
