#include "../defines.h"

#include <raylib.h>
#include <flecs.h>
#include <chipmunk.h>

#include "../components/display.h"
#include "../systems/scene.h"

#include "texture.h"
#include "sound.h"
#include "music.h"
#include "font.h"
#include "shader.h"
#include "data.h"
#include "component.h"
#include "entity.h"
#include "system.h"
#include "settings.h"
#include "debug.h"
#include "input.h"
#include "nuklear.h"

#include "game.h"

//==============================================================================

static ecs_world_t *_world = NULL;
static cpSpace *_space = NULL;

//==============================================================================

static void
_fini(ecs_world_t *world, void *context)
{
  if (IsAudioDeviceReady())
  {
    CloseAudioDevice();
  }
  CloseWindow();
  cpSpaceFree(_space);
  _space = NULL;
}

//------------------------------------------------------------------------------

static inline void _init_chipmunk()
{
  _space = cpSpaceNew();
}

//------------------------------------------------------------------------------

static inline void _init_flecs()
{
  _world = ecs_init();
  ecs_atfini(_world, _fini, NULL);
  // ecs_set_threads(world, 12);
  // TODO: pre-allocate memory with ecs_dim and ecs_dim_type
}

//------------------------------------------------------------------------------

static inline void _init_raylib()
{
#ifdef RELEASE
  SetTraceLogLevel(LOG_ERROR);
#endif
#ifdef DEBUG
  SetTraceLogLevel(LOG_TRACE);
#endif

  int flags = FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT;

#ifdef MAC
  flags = flags | FLAG_WINDOW_HIGHDPI;
#endif
#ifdef DEBUG
  flags = flags | FLAG_WINDOW_RESIZABLE;
#endif

  SetConfigFlags(flags);
  SetTargetFPS(60);
  SetExitKey(0);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
  InitAudioDevice();

  for (int i = 0; i < 2; ++i)
  {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextEx(GetFontDefault(), "LOADING...", (Vector2){20, 20}, 24, 0, YELLOW);
    EndDrawing();
  }
}

//------------------------------------------------------------------------------

static inline void _init_managers()
{
  texture_manager_init(_world);
  sound_manager_init(_world);
  music_manager_init(_world);
  shader_manager_init(_world);
  font_manager_init(_world);
  data_manager_init(_world);
  component_manager_init(_world);
  entity_manager_init(_world);
  debug_manager_init(_world);
  settings_manager_init(_world);
  input_manager_init(_world);
  nuklear_manager_init(_world);
  system_manager_init(_world);
}

//------------------------------------------------------------------------------

static inline void _init_game()
{
  SetWindowIcon(GetTextureData(*texture_manager_get(TEXTURE_SHIP)));
#ifdef RELEASE
  spawn_scene(_world, SCENE_SPLASH);
#endif
#ifdef DEBUG
  spawn_scene(_world, SCENE_SPLASH);
#endif
  ecs_singleton_set(_world, Display, {.border = BLACK, .background = WHITE, .raster = {0, 0, RASTER_WIDTH, RASTER_HEIGHT}});
}

//==============================================================================

void game_manager_init(void)
{
  _init_raylib();
  _init_flecs();
  _init_chipmunk();
  _init_managers();
  _init_game();
}

//------------------------------------------------------------------------------

void game_manager_loop(void)
{
  bool running = true;
  while (running)
  {
    float delta = GetFrameTime();
    cpSpaceStep(_space, delta);
    running = ecs_progress(_world, delta);
  }
}

//------------------------------------------------------------------------------

void game_manager_fini(void)
{
  ecs_fini(_world);
  _world = NULL;
}
