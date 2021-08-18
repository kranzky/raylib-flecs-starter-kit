#include <raylib.h>

#include "../defines.h"

#include "../managers/texture.h"
#include "../managers/entity.h"

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
  if (!debug->enabled)
    return;
  entity_manager_spawn_debug(debug->version);
  entity_manager_spawn_debug("hello");
  entity_manager_spawn_debug("world");
}

//------------------------------------------------------------------------------

void debug_scene(ecs_iter_t *it)
{
}

//------------------------------------------------------------------------------

void debug_render(ecs_iter_t *it)
{
  Debug *debug = ecs_column(it, Debug, 1);
  Label *label = ecs_column(it, Label, 2);
  if (!debug->enabled)
    return;
  BeginTextureMode(*texture_manager_playfield());
  DrawFPS(RASTER_WIDTH - 90, 10);
  Rectangle underlay = {5, 5, 0, 0};
  float height = 0;
  for (int i = 0; i < it->count; ++i)
  {
    Vector2 size = MeasureTextEx(GetFontDefault(), label[i].text, DEBUG_SIZE, 4);
    if (size.x > underlay.width)
      underlay.width = size.x;
    underlay.height += size.y;
    height = size.y;
  }
  underlay.width += 10;
  underlay.height += 10;
  DrawRectangleRec(underlay, (Color){0, 0, 0, 128});
  // TODO: draw underlay rectangle
  Vector2 position = {10, 10};
  for (int i = 0; i < it->count; ++i)
  {
    DrawTextEx(GetFontDefault(), label[i].text, position, DEBUG_SIZE, 4, RED);
    position.y += height;
    ecs_delete(it->world, it->entities[i]);
  }
  EndTextureMode();
}
