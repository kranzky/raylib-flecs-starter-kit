#include <tinyfiledialogs.h>

#include "splash.h"

#include "../managers/entity.h"
#include "../managers/nuklear.h"

#include "../defines.h"

//==============================================================================

static bool _quit = false;

//==============================================================================

static void _play_game(Widget *widget)
{
  TraceLog(LOG_TRACE, "PLAY GAME");
}

//------------------------------------------------------------------------------

static void _quit_game(Widget *widget)
{
  TraceLog(LOG_TRACE, "QUIT GAME");
}

//------------------------------------------------------------------------------

void init_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_label(world, parent, FONT_CLOVER, "Title Screen", ALIGN_CENTRE, VALIGN_MIDDLE, 50, position, ORANGE);
  ecs_entity_t window = nuklear_window(world, parent, "Hello Window", 100, 100, 300, 200);
  nuklear_label(world, window, "Mister Label");
  nuklear_separator(world, window);
  nuklear_button(world, window, "Play Game", _play_game);
  nuklear_button(world, window, "Quit Game", _quit_game);
  window = nuklear_window(world, parent, "Another One", 50, 150, 400, 300);
  nuklear_label(world, window, "Why Not Work");
  nuklear_label(world, window, "Another One");
  nuklear_separator(world, window);
  nuklear_label(world, window, "One");
  nuklear_label(world, window, "Two");
  nuklear_label(world, window, "Three");
  nuklear_separator(world, window);
  nuklear_label(world, window, "Single");
  nuklear_separator(world, window);
  nuklear_label(world, window, "Why Not Work");
  nuklear_label(world, window, "Another One");
  _quit = false;
}

//------------------------------------------------------------------------------

bool update_title(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings, ecs_entity_t parent)
{
  _quit = input->quit && (tinyfd_messageBox(GAME_NAME, "Really quit?", "yesno", "warning", 1) == 1);
  if (_quit)
    return false;
  return true;
}

//------------------------------------------------------------------------------

void fini_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  if (_quit)
    ecs_quit(world);
  else
    entity_manager_spawn_scene(world, SCENE_LEVEL);
}
