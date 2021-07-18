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
  _world = world;
}
