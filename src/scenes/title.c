#include <tinyfiledialogs.h>

#include "splash.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

static bool _quit = false;

//==============================================================================

void init_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_label(world, parent, FONT_CLOVER, "Title Screen", ALIGN_CENTRE, VALIGN_MIDDLE, 50, position, ORANGE);
  _quit = false;
}

//------------------------------------------------------------------------------

bool update_title(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings, ecs_entity_t parent)
{
  _quit = input->quit && (tinyfd_messageBox(NULL, "Really quit?", "yesno", "warning", 1) == 1);
  if (_quit || input->select)
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
