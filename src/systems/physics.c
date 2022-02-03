#include "../components/physics.h"
#include "../components/time.h"
#include "../components/collision.h"
#include "../components/physical.h"
#include "../components/tinted.h"

#include "../managers/physics.h"
#include "../managers/entity.h"

#include "physics.h"

#include "../helpers.h"

//==============================================================================

void physics_update(ecs_iter_t *it)
{
  Time *time = ecs_term(it, Time, 1);
  Physics *physics = ecs_term(it, Physics, 2);
  cpSpaceSetUserData(physics->space, it->world);
  cpSpaceStep(physics->space, time->delta);
}

//------------------------------------------------------------------------------

void physics_collide(ecs_iter_t *it)
{
  for (int i = 0; i < it->count; ++i)
  {
    ecs_delete(it->world, it->entities[i]);
  }
}
