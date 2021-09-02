#include "splash.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

void init_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_image(world, parent, TEXTURE_LOGO, 1, (Vector2){0.5 * RASTER_WIDTH, 0.5 * RASTER_HEIGHT}, WHITE);
}

//------------------------------------------------------------------------------

bool update_splash(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings, ecs_entity_t parent)
{
  return scene->time < 3 && !input->select && !input->quit;
}

//------------------------------------------------------------------------------

void fini_splash(ecs_world_t *world, const Scene *scene, ecs_entity_t parent)
{
  entity_manager_spawn_scene(world, SCENE_TITLE);
}
