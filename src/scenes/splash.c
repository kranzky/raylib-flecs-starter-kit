#include "splash.h"

#include "../managers/entity.h"

#include "../defines.h"

//==============================================================================

void init_splash(ecs_world_t *world, const Scene *scene)
{
  Vector2 position = {RASTER_WIDTH * 0.5, RASTER_HEIGHT * 0.5};
  entity_manager_spawn_label(FONT_CLOVER, "Splash Screen", ALIGN_CENTRE, VALIGN_MIDDLE, 50, position, PURPLE);
}

//------------------------------------------------------------------------------

bool update_splash(ecs_world_t *world, const Scene *scene)
{
  // wait for a certain amount of time
  return true;
}

//------------------------------------------------------------------------------

void fini_splash(ecs_world_t *world, const Scene *scene)
{
  // spawn the title screen
}
