#include <tinyfiledialogs.h>

#include "title.h"

#include "../systems/scene.h"

#include "../managers/entity.h"
#include "../managers/gui.h"

#include "../defines.h"

//==============================================================================

static bool _quit = false;
static bool _play = false;

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

ecs_entity_t spawn_title(ecs_world_t *world, int value)
{
  return entity_manager_spawn_scene(world, SCENE_TITLE, DARKGRAY, MAX_SHADERS, MAX_TEXTURES);
}

//------------------------------------------------------------------------------

void init_title(ecs_world_t *world, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, 50};
  entity_manager_spawn_label(world, parent, FONT_CLOVER, "Starter Kit", ALIGN_CENTRE, VALIGN_TOP, 50, position, ORANGE);
  ecs_entity_t window = gui_window(world, parent, "Starter Kit", 0.5 * (RASTER_WIDTH - 480), 150, 480, 240, 0, 5);
  gui_button(world, window, "Meesta Collecta", 0, _play_game);
  gui_button(world, window, "99 Sheep", 0, _play_game);
  gui_button(world, window, "Scorched Golf", 0, _play_game);
  gui_button(world, window, "Tanks Very Much", 0, _play_game);
  gui_button(world, window, "Quit Game", 0, _quit_game);
  _quit = false;
  _play = false;
}

//------------------------------------------------------------------------------

bool update_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent, const Input *input, const Time *time, const Settings *settings)
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

void fini_title(ecs_world_t *world, const Scene *scene)
{
  if (_quit)
  {
    ecs_quit(world);
  }
  else
  {
    spawn_scene(world, SCENE_LEVEL, 0);
  }
}
