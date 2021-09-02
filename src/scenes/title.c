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

static void _play_game(ecs_world_t *world, Widget *widget)
{
  _play = true;
}

//------------------------------------------------------------------------------

static void _quit_game(ecs_world_t *world, Widget *widget)
{
  _quit = true;
}

//------------------------------------------------------------------------------

static void _set_volume(ecs_world_t *world, Widget *widget)
{
  music_manager_volume(world, _music, widget->value * 0.01);
}

//------------------------------------------------------------------------------

void init_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, 150};
  entity_manager_spawn_label(world, parent, FONT_CLOVER, "Title Screen", ALIGN_CENTRE, VALIGN_TOP, 50, position, ORANGE);
  _music = entity_manager_spawn_music(world, MUSIC_ROCK_VOMIT, 1);
  ecs_entity_t window = nuklear_window(world, parent, "Starter Kit", 100, 250, 440, 150);
  nuklear_slider(world, window, 100, _set_volume);
  nuklear_separator(world, window);
  nuklear_button(world, window, "Play Game", _play_game);
  nuklear_button(world, window, "Quit Game", _quit_game);
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
  music_manager_stop(world, _music);
  if (_quit)
    ecs_quit(world);
  else
    entity_manager_spawn_scene(world, SCENE_LEVEL);
}
