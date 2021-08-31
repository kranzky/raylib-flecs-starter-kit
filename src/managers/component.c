#include "../components/scene.h"
#include "../components/spatial.h"
#include "../components/label.h"
#include "../components/tinted.h"
#include "../components/input.h"
#include "../components/settings.h"
#include "../components/debug.h"
#include "../components/nuklear.h"

#include "component.h"

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
}

//------------------------------------------------------------------------------

void component_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ECS_COMPONENT_DEFINE(world, Scene);
  ECS_COMPONENT_DEFINE(world, Spatial);
  ECS_COMPONENT_DEFINE(world, Label);
  ECS_COMPONENT_DEFINE(world, Tinted);
  ECS_COMPONENT_DEFINE(world, Input);
  ECS_COMPONENT_DEFINE(world, Settings);
  ECS_COMPONENT_DEFINE(world, Debug);
  ECS_COMPONENT_DEFINE(world, Nuklear);
}
