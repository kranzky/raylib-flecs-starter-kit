#include <raylib.h>
#include <raymath.h>

#include "../defines.h"

#include "../components/input.h"
#include "../components/viewport.h"

#include "../managers/physics.h"

#include "viewport.h"

//==============================================================================

void update_viewport(ecs_iter_t *it)
{
  Input *input = ecs_term(it, Input, 1);
  Viewport *viewport = ecs_term(it, Viewport, 2);
  for (int i = it->count - 1; i >= 0; --i)
  {
    viewport[i].active = false;
    viewport[i].pointer = (Vector2){
        viewport[i].size.x * (input->pointer.x - viewport[i].dst.x) / viewport[i].dst.width,
        viewport[i].size.y * (viewport[i].dst.height - input->pointer.y + viewport[i].dst.y) / viewport[i].dst.height};
  }
}
