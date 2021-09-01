#include "system.h"

#include "../systems/scene.h"
#include "../systems/render.h"
#include "../systems/input.h"
#include "../systems/nuklear.h"

#ifdef DEBUG
#include "../systems/debug.h"
#endif

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
  ECS_SYSTEM(world, process_input, EcsPreUpdate, [out] $Input, [inout] $Settings);
  ECS_SYSTEM(world, nuklear_input, EcsPreUpdate, [inout] $Nuklear, [in] $Input);
  ECS_SYSTEM(world, update_scene, EcsOnUpdate, [inout] Scene, [in] $Input, [inout] $Settings, [out] :*);
  ECS_SYSTEM(world, nuklear_update, EcsOnUpdate, [inout] $Nuklear, [in] PARENT:Window, [inout] ?Widget);
  ECS_SYSTEM(world, render_labels, EcsOnStore, [in] Label, [inout] Spatial, [in] Tinted);
  ECS_SYSTEM(world, nuklear_render, EcsOnStore, [inout] $Nuklear);

#ifdef DEBUG
  ECS_SYSTEM(world, debug_input, EcsPostUpdate, [in] $Input, [out] $Debug, [out] :*);
  ECS_SYSTEM(world, debug_scene, EcsPostUpdate, [in] Scene, [out] $Debug, [out] :*);
  ECS_SYSTEM(world, debug_render, EcsOnStore, [in] Label, [in] $Debug, DebugTag);
#endif
}
