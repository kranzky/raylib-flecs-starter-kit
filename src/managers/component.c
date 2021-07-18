#include "../components/scene.h"
#include "../components/spatial.h"
#include "../components/label.h"

#include "component.h"

//==============================================================================

static char *_version = "DEBUG BUILD";

//==============================================================================

/*
static void _load_options(ecs_world_t *world)
{
  GameOptions *options = ecs_singleton_get_mut(world, GameOptions);
  int value = 0;
  value = LoadStorageValue(0);
  if (value == 0)
  {
    TraceLog(LOG_WARNING, "Unable to load music option");
  }
  else
  {
    options->play_music = (value == 2) ? true : false;
  }
  value = LoadStorageValue(1);
  if (value == 0)
  {
    TraceLog(LOG_WARNING, "Unable to load invert y axis option");
  }
  else
  {
    options->invert_y_axis = (value == 2) ? true : false;
  }
  value = LoadStorageValue(2);
  if (value == 0)
  {
    TraceLog(LOG_WARNING, "Unable to load fullscreen option");
  }
  else
  {
    options->fullscreen = (value == 2) ? true : false;
  }
  ecs_singleton_modified(world, GameOptions);
}
*/

//------------------------------------------------------------------------------

/*
static void _save_options(ecs_world_t *world)
{
  const GameOptions *options = ecs_singleton_get(world, GameOptions);
  if (!SaveStorageValue(0, options->play_music ? 2 : 1))
  {
    TraceLog(LOG_WARNING, "Unable to save music option");
  }
  if (!SaveStorageValue(1, options->invert_y_axis ? 2 : 1))
  {
    TraceLog(LOG_WARNING, "Unable to save invert y axis option");
  }
  if (!SaveStorageValue(2, options->fullscreen ? 2 : 1))
  {
    TraceLog(LOG_WARNING, "Unable to save fullscreen option");
  }
}
*/

//------------------------------------------------------------------------------

static void _fini(ecs_world_t *world, void *context)
{
  // _save_options(world);
#ifdef RELEASE
  UnloadFileText((unsigned char *)_version);
#endif
}

//------------------------------------------------------------------------------

void component_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ECS_COMPONENT_DEFINE(world, Scene);
  ECS_COMPONENT_DEFINE(world, Spatial);
  ECS_COMPONENT_DEFINE(world, Label);

#ifdef RELEASE
  _version = LoadFileText("./res/VERSION");
#endif

  /*
  ecs_singleton_set(world, Input, {0});
  ecs_singleton_set(world, Spatial, {0});
  ecs_singleton_set(world, Projection, {0});
  ecs_singleton_set(world, GameOptions, {.play_music = true, .stage = MAX_STAGES, .fade = 1, .overlay = true});
  ecs_singleton_set(world, Debug, {.version = _version});
  */

  // _load_options(world);
}
