#include <tinyfiledialogs.h>

#include "splash.h"

#include "../managers/entity.h"
#include "../managers/nuklear.h"

#include "../defines.h"

//==============================================================================

static bool _quit = false;
static bool _play = false;
static ecs_entity_t _music = 0;

//==============================================================================

static void _play_game(Widget *widget)
{
  _play = true;
}

//------------------------------------------------------------------------------

static void _quit_game(Widget *widget)
{
  _quit = true;
}

//------------------------------------------------------------------------------

static void _set_volume(Widget *widget)
{
  TraceLog(LOG_TRACE, "SET VOLUME %0.1f", widget->value);
}

//------------------------------------------------------------------------------

void init_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_label(world, parent, FONT_CLOVER, "Title Screen", ALIGN_CENTRE, VALIGN_TOP, 50, position, ORANGE);
  _music = entity_manager_spawn_music(world, MUSIC_ROCK_VOMIT, 1);
  ecs_entity_t window = nuklear_window(world, parent, "Hello Window", 100, 100, 300, 200);
  nuklear_label(world, window, "Mister Label");
  nuklear_separator(world, window);
  nuklear_button(world, window, "Play Game", _play_game);
  nuklear_button(world, window, "Quit Game", _quit_game);
  nuklear_separator(world, window);
  nuklear_slider(world, window, 42, _set_volume);
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
  _play = false;
}

//------------------------------------------------------------------------------

bool update_title(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings, ecs_entity_t parent)
{
  _quit |= input->quit;
#ifdef RELEASE
  if (_quit)
    _quit = (tinyfd_messageBox(GAME_NAME, "Really quit?", "yesno", "warning", 1) == 1);
#endif
  if (_quit || _play)
    return false;
  return true;
}

//------------------------------------------------------------------------------

void fini_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  // TODO: make music fade out instead
  ecs_delete(world, _music);
  if (_quit)
    ecs_quit(world);
  else
    entity_manager_spawn_scene(world, SCENE_LEVEL);
}
