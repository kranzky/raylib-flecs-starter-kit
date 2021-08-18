#include <raylib.h>

#include "../defines.h"

#include "../managers/texture.h"

#include "../components/debug.h"
#include "../components/input.h"
#include "../components/scene.h"

#include "debug.h"

//==============================================================================

void debug_input(ecs_iter_t *it)
{
  Input *input = ecs_column(it, Input, 1);
  Debug *debug = ecs_column(it, Debug, 2);
  if (input->toggle_debug)
    debug->enabled = !debug->enabled;
}

//------------------------------------------------------------------------------

void debug_scene(ecs_iter_t *it)
{
}

//------------------------------------------------------------------------------

void debug_render(ecs_iter_t *it)
{
  Debug *debug = ecs_column(it, Debug, 1);
  if (!debug->enabled)
    return;
  BeginTextureMode(*texture_manager_playfield());
  DrawFPS(RASTER_WIDTH - 90, 10);
  EndTextureMode();
}
