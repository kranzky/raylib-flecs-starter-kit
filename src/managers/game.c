#include "../defines.h"

#include <raylib.h>
#include <flecs.h>
#include <chipmunk.h>

#include "../components/settings.h"

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
}

//------------------------------------------------------------------------------

static inline void _show_loading_screen()
{
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

//==============================================================================

void game_manager_init(void)
{
  _init_chipmunk();
  _init_flecs();
  _init_raylib();
  _show_loading_screen();
  _init_managers();
}

//------------------------------------------------------------------------------

void game_manager_loop(void)
{
  bool running = true;
  RenderTexture2D *playfield = texture_manager_playfield();
  Rectangle src = {0.0f, 0.0f, (float)RASTER_WIDTH, (float)-RASTER_HEIGHT};

  SetWindowIcon(GetTextureData(*texture_manager_get(TEXTURE_SHIP)));

#ifdef RELEASE
  entity_manager_spawn_scene(_world, SCENE_SPLASH);
#endif
#ifdef DEBUG
  entity_manager_spawn_scene(_world, SCENE_SPLASH);
#endif

  while (running)
  {
    BeginTextureMode(*playfield);
    ClearBackground(BLUE);
    EndTextureMode();

    cpSpaceStep(_space, GetFrameTime());
    running = ecs_progress(_world, GetFrameTime());

    const Settings *settings = ecs_singleton_get(_world, Settings);

    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawTexturePro(playfield->texture, src, settings->bounds, (Vector2){0}, 0.0f, WHITE);
    EndDrawing();
  }
}

//------------------------------------------------------------------------------

void game_manager_fini(void)
{
  ecs_fini(_world);
  _world = NULL;
}
