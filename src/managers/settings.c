#include <raylib.h>

#include "../components/settings.h"

#include "settings.h"

//==============================================================================

static bool _load_bool(Settings *settings, unsigned int slot)
{
  return (LoadStorageValue(slot) == 1);
}

//------------------------------------------------------------------------------

static void _load_settings(ecs_world_t *world)
{
  Settings *settings = ecs_singleton_get_mut(world, Settings);
  settings->music = _load_bool(settings, 0);
  settings->fullscreen = _load_bool(settings, 1);
  ecs_singleton_modified(world, Settings);
}

//------------------------------------------------------------------------------

static void _save_bool(Settings *settings, unsigned int slot, bool value)
{
  SaveStorageValue(slot, value ? 1 : 0);
}

//------------------------------------------------------------------------------
static void _save_settings(ecs_world_t *world)
{
  Settings *settings = ecs_singleton_get_mut(world, Settings);
  _save_bool(settings, 0, settings->music);
  _save_bool(settings, 1, settings->fullscreen);
}

//------------------------------------------------------------------------------

static void _fini(ecs_world_t *world, void *context)
{
  _save_settings(world);
}

//------------------------------------------------------------------------------

void settings_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ecs_singleton_set(world, Settings, {.music = true, .fullscreen = false, .gamepad = -1});
  _load_settings(world);
}
