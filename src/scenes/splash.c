#include "splash.h"

#include "../systems/scene.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

void spawn_splash(ecs_world_t *world)
{
  entity_manager_spawn_scene(world, SCENE_SPLASH, WHITE, MAX_SHADERS);
}

//------------------------------------------------------------------------------

void init_splash(ecs_world_t *world, ecs_entity_t parent)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_image(world, parent, TEXTURE_LOGO, 1, position, WHITE);
  entity_manager_spawn_sound(world, parent, SOUND_KRANZKY, 1);
}

//------------------------------------------------------------------------------

bool update_splash(ecs_world_t *world, const Scene *scene, const Input *input, const Settings *settings)
{
  return scene->time < 2 && !input->select && !input->quit;
}

//------------------------------------------------------------------------------

void fini_splash(ecs_world_t *world, const Scene *scene)
{
  spawn_scene(world, SCENE_TITLE);
}
