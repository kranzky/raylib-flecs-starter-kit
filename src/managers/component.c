#include "../components/scene.h"
#include "../components/spatial.h"
#include "../components/label.h"
#include "../components/tinted.h"

#include "component.h"

//==============================================================================

static char *_version = "DEBUG BUILD";

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
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
  ECS_COMPONENT_DEFINE(world, Tinted);

#ifdef RELEASE
  _version = LoadFileText("./res/VERSION");
#endif
}
