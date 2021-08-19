#include "level.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

void init_level(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_label(parent, FONT_CLOVER, "Game Level", ALIGN_CENTRE, VALIGN_MIDDLE, 50, position, GREEN);
}

//------------------------------------------------------------------------------

bool update_level(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings, ecs_entity_t parent)
{
  return !input->quit;
}

//------------------------------------------------------------------------------

void fini_level(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  entity_manager_spawn_scene(SCENE_TITLE);
}
