#include "system.h"

#include "../components/viewport.h"

#include "../systems/scene.h"
#include "../systems/render.h"
#include "../systems/input.h"
#include "../systems/gui.h"
#include "../systems/audio.h"
#include "../systems/physics.h"
#include "../systems/viewport.h"
#include "../systems/time.h"

#ifdef DEBUG
#include "../systems/debug.h"
#endif

//==============================================================================

#ifdef __clang__
#pragma GCC diagnostic ignored "-Wdollar-in-identifier-extension"
#endif

static ecs_query_t *_viewport_query = NULL;

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
}

//------------------------------------------------------------------------------

void system_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ECS_SYSTEM(world, refresh_display, EcsOnLoad, [out] $Display);
  ECS_SYSTEM(world, progress_time, EcsOnLoad, [inout] $Time);
  ECS_SYSTEM(world, process_input, EcsOnLoad, [out] $Input, [inout] $Settings, [in] $Display);
  ECS_SYSTEM(world, gui_input, EcsPostLoad, [inout] $Interface, [in] $Input, [in] $Settings, [in] Window);
  ECS_SYSTEM(world, gui_update, EcsPreUpdate, [inout] $Interface, [in] PARENT:Window, [inout] ?Widget);
  ECS_SYSTEM(world, transition, EcsPreUpdate, [in] $Time, [inout] Transition, [out] :*);
  ECS_SYSTEM(world, state_machine, EcsPreUpdate, [in] $Time, [inout] Stateful, [out] :*);
  ECS_SYSTEM(world, update_viewport, EcsOnUpdate, [in] $Input, [inout] Viewport);
  ECS_SYSTEM(world, update_scene, EcsOnUpdate, [inout] Scene, [inout] Stateful, [out] Transition, [inout] $Time, [in] $Input, [inout] $Settings, [out] :*);
  ECS_SYSTEM(world, physics_update, EcsOnValidate, [in] $Time, [inout] $Physics, [out] :Collision);
  ECS_SYSTEM(world, physics_collide, EcsOnValidate, [in] $Physics, [inout] Collision, [out] :*);
#ifdef DEBUG
  ECS_SYSTEM(world, debug_input, EcsPostUpdate, [in] $Input, [out] $Debug, [out] :*);
  ECS_SYSTEM(world, debug_scene, EcsPostUpdate, [in] Scene, [in] Stateful, [out] $Debug, [out] :*);
#endif
  ECS_SYSTEM(world, play_music, EcsPreStore, [inout] Track);
  ECS_SYSTEM(world, play_sounds, EcsPreStore, [inout] Audible, [out] :*);
  ECS_SYSTEM(world, animate, EcsOnStore, [inout] Animated, [out] Renderable);
  ECS_SYSTEM(world, render_scene, EcsOnStore, [in] Scene, [in] Stateful, [in] Transition, [out] $Display, [in] $Time);
  ECS_SYSTEM(world, render_viewports, EcsOnStore, [in] Viewport);
  ECS_SYSTEM(world, render_physical, EcsOnStore, [in] Renderable, [in] Physical, [in] Tinted, [in] ?Transition);
  ECS_SYSTEM(world, gui_render, EcsOnStore, [inout] $Interface);
  ECS_SYSTEM(world, render_images, EcsOnStore, [in] Renderable, [in] Spatial, [in] Tinted);
#ifdef DEBUG
  ECS_SYSTEM(world, debug_physics, EcsOnStore, [in] $Physics, [in] $Debug, [inout] Viewport);
  ECS_SYSTEM(world, debug_render, EcsOnStore, [in] Label, [in] $Debug, DebugTag, [out] :*);
  ECS_SYSTEM(world, debug_display, EcsOnStore, [in] $Display, [in] $Debug);
#endif
  ECS_SYSTEM(world, composite_display, EcsOnStore, [in] $Display);

  _viewport_query = ecs_get_system_query(world, render_viewports);
}

//------------------------------------------------------------------------------

ecs_query_t *system_manager_viewport_query(void)
{
  return _viewport_query;
}

