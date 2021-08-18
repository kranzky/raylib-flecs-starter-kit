#include "../components/input.h"

#include "input.h"

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
}

//------------------------------------------------------------------------------

void input_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ecs_singleton_set(world, Input, {0});
}
