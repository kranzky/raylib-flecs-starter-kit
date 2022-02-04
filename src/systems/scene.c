#include <raylib.h>
#include <raymath.h>

#include "../defines.h"

#include "../components/input.h"
#include "../components/settings.h"
#include "../components/time.h"
#include "../components/stateful.h"
#include "../components/transition.h"

#include "../managers/entity.h"
#include "../managers/system.h"

#include "../scenes/splash.h"
#include "../scenes/title.h"
#include "../scenes/level.h"

#include "scene.h"

//==============================================================================

static ecs_entity_t _paused = 0;
static ecs_entity_t _music = 0;
static ecs_entity_t _menu = false;

typedef ecs_entity_t (*spawnScene)(ecs_world_t *world, int value);
typedef void (*initScene)(ecs_world_t *world, ecs_entity_t entity);
typedef bool (*updateScene)(ecs_world_t *world, const Scene *scene, ecs_entity_t entity, const Input *input, const Time *time, const Settings *settings);
typedef void (*finiScene)(ecs_world_t *world, const Scene *scene);

typedef struct JumpTarget
{
  spawnScene spawn;
  initScene init;
  updateScene update;
  finiScene fini;
} JumpTarget;

JumpTarget _targets[] = {[SCENE_TITLE] = {.spawn = spawn_title, .init = init_title, .update = update_title, .fini = fini_title},
                         [SCENE_SPLASH] = {.spawn = spawn_splash, .init = init_splash, .update = update_splash, .fini = fini_splash},
                         [SCENE_LEVEL] = {.spawn = spawn_level, .init = init_level, .update = update_level, .fini = fini_level}};

//==============================================================================

static inline void _init(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  _targets[scene->id].init(world, entity);
}

//------------------------------------------------------------------------------

static inline bool _update(ecs_world_t *world, Scene *scene, ecs_entity_t entity, const Input *input, const Time *time, const Settings *settings)
{
  return _targets[scene->id].update(world, scene, entity, input, time, settings);
}

//------------------------------------------------------------------------------

static inline void _fini(ecs_world_t *world, Scene *scene, ecs_entity_t entity)
{
  _targets[scene->id].fini(world, scene);
}

//==============================================================================

ecs_entity_t spawn_scene(ecs_world_t *world, SceneName id, int value)
{
  return _targets[id].spawn(world, value);
}

//------------------------------------------------------------------------------

static inline void _pause(ecs_world_t *world)
{
  if (_paused == 0)
  {
    _paused = entity_manager_spawn_label(world, 0, FONT_CLOVER, "PAUSED", ALIGN_CENTRE, VALIGN_MIDDLE, 100, (Vector2){RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5}, DARKGRAY);
    ShowCursor();
    if (_music != 0)
    {
      music_manager_mute(world, _music);
    }
  }
}

//------------------------------------------------------------------------------

static inline void _unpause(ecs_world_t *world)
{
  if (_paused != 0)
  {
    HideCursor();
    ecs_delete(world, _paused);
    _paused = 0;
    if (_music != 0)
    {
      music_manager_unmute(world, _music);
    }
  }
}

//------------------------------------------------------------------------------

static inline void _process_input(ecs_world_t *world, Input *input, Time *time, bool allow_pause)
{
  bool paused = time->paused;
  if (input->toggle_pause || (input->quit && time->paused))
    time->paused = !time->paused;
  if (input->toggle_fullscreen)
  {
    time->paused = true;
    ToggleFullscreen();
  }
  if (IsWindowResized() || !IsWindowFocused())
    time->paused = true;
  if (!allow_pause)
    time->paused = false;
  if (!paused && time->paused)
    _pause(world);
  else if (paused && !time->paused)
    _unpause(world);
}

//------------------------------------------------------------------------------

void update_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_term(it, Scene, 1);
  Stateful *stateful = ecs_term(it, Stateful, 2);
  Transition *transition = ecs_term(it, Transition, 3);
  Time *time = ecs_term(it, Time, 4);
  Input *input = ecs_term(it, Input, 5);
  Settings *settings = ecs_term(it, Settings, 6);
  bool allow_paused = true;
  for (int i = 0; i < it->count; ++i)
  {
    switch (scene[i].id)
    {
    case SCENE_SPLASH:
    case SCENE_TITLE:
    {
      allow_paused = false;
    }
    }
  }
  _process_input(it->world, input, time, allow_paused);
  for (int i = 0; i < it->count; ++i)
  {
    switch (stateful[i].state)
    {
    case STATE_CREATED:
    {
      _init(it->world, &scene[i], it->entities[i]);
      break;
    }
    case STATE_STARTING:
    {
      if (stateful[i].transitioned)
      {
        transition[i].complete = false;
        transition[i].id = TRANSITION_FADE_IN;
        transition[i].time = 0;
        transition[i].fade = 0;
        switch (scene[i].id)
        {
        case SCENE_TITLE:
        {
          if (_music != 0 && !_menu)
          {
            music_manager_stop(it->world, _music);
            _music = 0;
          }
          if (_music == 0)
          {
            _music = entity_manager_spawn_music(it->world, MUSIC_ROCK_VOMIT, 1);
          }
          _menu = true;
          break;
        }
        }
      }
      break;
    }
    case STATE_RUNNING:
    {
      if (!_update(it->world, &scene[i], it->entities[i], input, time, settings))
        stateful[i].run_time = 0;
      break;
    }
    case STATE_STOPPING:
    {
      if (stateful[i].transitioned)
      {
        transition[i].complete = false;
        transition[i].id = TRANSITION_FADE_OUT;
        transition[i].time = 0;
      }
      break;
    }
    case STATE_STOPPED:
    {
      _fini(it->world, &scene[i], it->entities[i]);
      break;
    }
    }
    scene[i].time += time->delta;
  }
}

//------------------------------------------------------------------------------

void scene_volume(ecs_world_t *world, Widget *widget)
{
  music_manager_volume(world, _music, widget->value * 0.01);
}
