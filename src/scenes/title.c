#include "splash.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

void init_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_label(parent, FONT_CLOVER, "Title Screen", ALIGN_CENTRE, VALIGN_MIDDLE, 50, position, ORANGE);
}

//------------------------------------------------------------------------------

bool update_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  return true;
}

//------------------------------------------------------------------------------

void fini_title(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
}
