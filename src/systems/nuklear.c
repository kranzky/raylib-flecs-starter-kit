#include <raylib.h>

#include "../components/input.h"

#include "../defines.h"

#include "nuklear.h"

//==============================================================================

void nuklear_input(ecs_iter_t *it)
{
  Input *input = ecs_column(it, Input, 1);
}

//------------------------------------------------------------------------------

void nuklear_render(ecs_iter_t *it)
{
}
