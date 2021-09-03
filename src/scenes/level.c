#include "level.h"

#include "../systems/scene.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

void spawn_level(ecs_world_t *world)
{
  entity_manager_spawn_scene(world, SCENE_LEVEL, RED, SHADER_DEFAULT);
}

//------------------------------------------------------------------------------

void init_level(ecs_world_t *world, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_label(world, parent, FONT_CLOVER, "Yay Play", ALIGN_CENTRE, VALIGN_MIDDLE, 50, position, GREEN);
}

//------------------------------------------------------------------------------

bool update_level(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings)
{
  return !input->quit;
}

//------------------------------------------------------------------------------

void fini_level(ecs_world_t *world, const Scene *scene)
{
  spawn_scene(world, SCENE_TITLE);
}
