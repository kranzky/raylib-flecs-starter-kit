#include "splash.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

void init_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  ecs_entity_t entity = entity_manager_spawn_label(FONT_CLOVER, "Splash Screen", ALIGN_CENTRE, VALIGN_MIDDLE, 50, position, PURPLE);
  ecs_add_entity(world, entity, ECS_CHILDOF | parent);
}

//------------------------------------------------------------------------------

bool update_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  return scene->time < 3;
}

//------------------------------------------------------------------------------

void fini_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  entity_manager_spawn_scene(SCENE_TITLE);
}
