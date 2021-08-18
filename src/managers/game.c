#include <raylib.h>
#include <flecs.h>

#include "../defines.h"

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

#include "game.h"

//==============================================================================

static ecs_world_t *_world = NULL;

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  if (IsAudioDeviceReady())
  {
    CloseAudioDevice();
  }
  CloseWindow();
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

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "StarterKit");
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
  system_manager_init(_world);
}

//==============================================================================

void game_manager_init(void)
{
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

#ifdef RELEASE
  entity_manager_spawn_scene(SCENE_SPLASH);
#endif
#ifdef DEBUG
  entity_manager_spawn_scene(SCENE_TITLE);
#endif

  while (running)
  {
    BeginTextureMode(*playfield);
    ClearBackground(BLUE);
    EndTextureMode();

    running = ecs_progress(_world, GetFrameTime());

    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();

#ifdef MAC
    window_width *= GetWindowScaleDPI().x;
    window_height *= GetWindowScaleDPI().y;
#endif

    float scale = MIN((float)window_width / RASTER_WIDTH, (float)window_height / RASTER_HEIGHT);
    Rectangle dst = {
        (window_width - ((float)RASTER_WIDTH * scale)) * 0.5f,
        (window_height - ((float)RASTER_HEIGHT * scale)) * 0.5f,
        (float)RASTER_WIDTH * scale,
        (float)RASTER_HEIGHT * scale};

    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawTexturePro(playfield->texture, src, dst, (Vector2){0}, 0.0f, WHITE);
    EndDrawing();
  }
}

//------------------------------------------------------------------------------

void game_manager_fini(void)
{
  ecs_fini(_world);
  _world = NULL;
}
