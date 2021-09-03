#include "system.h"

#include "../systems/scene.h"
#include "../systems/render.h"
#include "../systems/input.h"
#include "../systems/nuklear.h"
#include "../systems/audio.h"

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

  ECS_SYSTEM(world, refresh_display, EcsPreFrame, [out] $Display);
  ECS_SYSTEM(world, process_input, EcsOnLoad, [out] $Input, [inout] $Settings, [in] $Display);
  ECS_SYSTEM(world, nuklear_input, EcsPostLoad, [inout] $Nuklear, [in] $Input);
  ECS_SYSTEM(world, nuklear_update, EcsPreUpdate, [inout] $Nuklear, [in] PARENT:Window, [inout] ?Widget);
  ECS_SYSTEM(world, update_scene, EcsOnUpdate, [inout] Scene, [in] $Input, [inout] $Settings, [out] :*);
  // EcsOnValidate - physics
#ifdef DEBUG
  ECS_SYSTEM(world, debug_input, EcsPostUpdate, [in] $Input, [out] $Debug, [out] :*);
  ECS_SYSTEM(world, debug_scene, EcsPostUpdate, [in] Scene, [out] $Debug, [out] :*);
#endif
  ECS_SYSTEM(world, play_sounds, EcsPreStore, [inout] Audible);
  ECS_SYSTEM(world, play_music, EcsPreStore, [inout] Track);
  ECS_SYSTEM(world, render_scene, EcsOnStore, [in] Scene);
  ECS_SYSTEM(world, render_images, EcsOnStore, [in] Renderable, [in] Spatial, [in] Tinted);
  ECS_SYSTEM(world, render_labels, EcsOnStore, [in] Label, [in] Spatial, [in] Tinted);
  ECS_SYSTEM(world, nuklear_render, EcsOnStore, [inout] $Nuklear);
#ifdef DEBUG
  ECS_SYSTEM(world, debug_render, EcsOnStore, [in] Label, [in] $Debug, DebugTag);
#endif
  ECS_SYSTEM(world, composite_display, EcsPostFrame, [in] $Display);
}
