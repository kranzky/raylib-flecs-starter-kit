#include "splash.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

void init_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_label(parent, FONT_CLOVER, "Splash Screen", ALIGN_CENTRE, VALIGN_MIDDLE, 50, position, PURPLE);
}

//------------------------------------------------------------------------------

bool update_splash(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings, ecs_entity_t parent)
{
  return scene->time < 3 && !input->select && !input->quit;
}

//------------------------------------------------------------------------------

void fini_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  entity_manager_spawn_scene(SCENE_TITLE);
}
