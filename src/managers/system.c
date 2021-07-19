#include "system.h"

#include "../systems/scene.h"
#include "../systems/render.h"

//==============================================================================

#ifdef __clang__
#pragma GCC diagnostic ignored "-Wdollar-in-identifier-extension"
#endif

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
}

//------------------------------------------------------------------------------

void system_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ECS_SYSTEM(world, update_scene, EcsOnUpdate, [inout] Scene, [out] :*);
  ECS_SYSTEM(world, render_labels, EcsOnStore, [in] Label, [inout] Spatial, [in] Tinted);
}
