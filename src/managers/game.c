#include "../defines.h"

#include <raylib.h>
#include <flecs.h>

#include "../components/display.h"
#include "../components/time.h"
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
#include "gui.h"
#include "physics.h"

#include "game.h"

//==============================================================================

static ecs_world_t *_world = NULL;

//==============================================================================

static void
_fini(ecs_world_t *world, void *context)
{
  if (IsAudioDeviceReady())
  {
    CloseAudioDevice();
  }
  CloseWindow();
}

//------------------------------------------------------------------------------

static inline void _init_flecs(void)
{
  _world = ecs_init();
  ecs_atfini(_world, _fini, NULL);
  // ecs_set_entity_range(_world, 1, 1000);
  // ecs_set_threads(world, 12);
}

//------------------------------------------------------------------------------

static inline void _init_raylib(void)
{
#ifdef RELEASE
  SetTraceLogLevel(LOG_ERROR);
#endif
#ifdef DEBUG
  SetTraceLogLevel(LOG_TRACE);
#endif

  int flags = FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE;

#ifdef MAC
  flags = flags | FLAG_WINDOW_HIGHDPI;
#endif

  SetConfigFlags(flags);
  SetTargetFPS(60);
  SetExitKey(0);

  InitAudioDevice();
  if (!IsAudioDeviceReady())
  {
    TraceLog(LOG_WARNING, "Unable to initialise audio device :(");
  }

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
  if (!IsWindowReady())
  {
    TraceLog(LOG_ERROR, "Unable to initialise OpenGL context :(");
    return;
  }

  for (int i = 0; i < 2; ++i)
  {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextEx(GetFontDefault(), "LOADING...", (Vector2){20, 20}, 24, 0, YELLOW);
    EndDrawing();
  }

  char *mappings = LoadFileText("./res/gamecontrollerdb.txt");
  SetGamepadMappings(mappings);
  UnloadFileText((unsigned char *)mappings);
}

//------------------------------------------------------------------------------

static inline void _init_managers(void)
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
  gui_manager_init(_world);
  physics_manager_init(_world);
  system_manager_init(_world);
}

//------------------------------------------------------------------------------

static inline void _init_game(void)
{
  if (IsWindowReady())
  {
    SetWindowIcon(GetTextureData(*texture_manager_get(TEXTURE_SHIP)));
  }
  ecs_singleton_set(_world, Display, {.border = BLACK, .background = WHITE, .raster = {0, 0, RASTER_WIDTH, RASTER_HEIGHT}});
  ecs_singleton_set(_world, Time, {.scale = 1});
}

//------------------------------------------------------------------------------

static inline void _start_game(void)
{
#ifdef RELEASE
  spawn_scene(_world, SCENE_SPLASH, 0);
#endif
#ifdef DEBUG
  spawn_scene(_world, SCENE_TITLE, 0);
#endif
}

//==============================================================================

void game_manager_init(void)
{
  _init_raylib();
  _init_flecs();
  _init_managers();
  _init_game();
}

//------------------------------------------------------------------------------

void game_manager_loop(void)
{
  bool running = true;
  bool started = false;
  float time = 0;
  if (!IsWindowReady())
    return;
  while (running)
  {
    float delta = GetFrameTime();
    running = ecs_progress(_world, delta);
    time += delta;
    if (!started && time > 1)
    {
      _start_game();
      started = true;
    }
  }
}

//------------------------------------------------------------------------------

void game_manager_fini(void)
{
  ecs_fini(_world);
  _world = NULL;
#ifdef RELEASE
  SetTraceLogLevel(LOG_INFO);
#endif
  TraceLog(LOG_INFO, "Thank you for playing %s!", GAME_NAME);
}
