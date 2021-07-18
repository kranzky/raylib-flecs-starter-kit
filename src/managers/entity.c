#include <raymath.h>
#include <raylib.h>

#include "../defines.h"

#include "entity.h"

//==============================================================================

static ecs_world_t *_world = NULL;

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  _world = NULL;
}

//------------------------------------------------------------------------------

void entity_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ECS_TYPE_DEFINE(world, SceneType, Scene);
  _world = world;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_scene(SceneName id)
{
  ecs_entity_t e = ecs_new(_world, SceneType);
  ecs_set(_world, e, Scene, {.id = id});
  return e;
}
