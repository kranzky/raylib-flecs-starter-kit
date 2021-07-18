#include "component.h"

/*
#include "../components/spatial.h"
#include "../components/movable.h"
#include "../components/collidable.h"
#include "../components/controllable.h"
#include "../components/input.h"
#include "../components/renderable.h"
#include "../components/weaponised.h"
#include "../components/contact.h"
#include "../components/substance.h"
#include "../components/audible.h"
#include "../components/track.h"
#include "../components/stage.h"
#include "../components/text.h"
#include "../components/shader.h"
#include "../components/animated.h"
#include "../components/selectable.h"
#include "../components/options.h"
#include "../components/sprite.h"
#include "../components/projection.h"
#include "../components/debug.h"
#include "../components/offset.h"
#include "../components/damageable.h"
#include "../components/transition.h"
#include "../components/morph.h"
#include "../components/enemy.h"
*/

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
  /*
  ECS_COMPONENT_DEFINE(world, Spatial);
  ECS_COMPONENT_DEFINE(world, Movable);
  ECS_COMPONENT_DEFINE(world, Collidable);
  ECS_COMPONENT_DEFINE(world, Renderable);
  ECS_COMPONENT_DEFINE(world, Controllable);
  ECS_COMPONENT_DEFINE(world, Input);
  ECS_COMPONENT_DEFINE(world, Weaponised);
  ECS_COMPONENT_DEFINE(world, Contact);
  ECS_COMPONENT_DEFINE(world, Substance);
  ECS_COMPONENT_DEFINE(world, Audible);
  ECS_COMPONENT_DEFINE(world, Track);
  ECS_COMPONENT_DEFINE(world, Stage);
  ECS_COMPONENT_DEFINE(world, Text);
  ECS_COMPONENT_DEFINE(world, BackgroundShader);
  ECS_COMPONENT_DEFINE(world, Animated);
  ECS_COMPONENT_DEFINE(world, Selectable);
  ECS_COMPONENT_DEFINE(world, GameOptions);
  ECS_COMPONENT_DEFINE(world, Sprite);
  ECS_COMPONENT_DEFINE(world, Projection);
  ECS_COMPONENT_DEFINE(world, Debug);
  ECS_COMPONENT_DEFINE(world, Offset);
  ECS_COMPONENT_DEFINE(world, Damageable);
  ECS_COMPONENT_DEFINE(world, Transition);
  ECS_COMPONENT_DEFINE(world, Morph);
  ECS_COMPONENT_DEFINE(world, Enemy);
  */

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
