#include "../defines.h"

#include "../components/label.h"
#include "../components/spatial.h"
#include "../components/tinted.h"
#include "../components/renderable.h"
#include "../components/scene.h"

#include "../managers/texture.h"

#include "render.h"

//==============================================================================

void render_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_column(it, Scene, 1);
  RenderTexture2D *playfield = texture_manager_playfield();
  BeginTextureMode(*playfield);
  for (int i = 0; i < it->count; ++i)
  {
    if (scene[i].state != SCENE_STATE_RUNNING)
      continue;
    if (scene[i].shader == NULL)
      ClearBackground(scene[i].color);
  }
  EndTextureMode();
}

//------------------------------------------------------------------------------

void render_labels(ecs_iter_t *it)
{
  Label *label = ecs_column(it, Label, 1);
  Spatial *spatial = ecs_column(it, Spatial, 2);
  Tinted *tinted = ecs_column(it, Tinted, 3);
  RenderTexture2D *playfield = texture_manager_playfield();
  BeginTextureMode(*playfield);
  for (int i = 0; i < it->count; ++i)
  {
    Vector2 size = MeasureTextEx(*label[i].font, label[i].text, label[i].size, 0);
    Vector2 position = spatial[i].position;
    switch (label[i].align)
    {
    case ALIGN_LEFT:
      break;
    case ALIGN_CENTRE:
      position.x -= 0.5 * size.x;
      break;
    case ALIGN_RIGHT:
      position.x -= size.x;
      break;
    default:
      TraceLog(LOG_WARNING, "bad text align");
    }
    switch (label[i].valign)
    {
    case VALIGN_TOP:
      break;
    case VALIGN_MIDDLE:
      position.y -= 0.5 * size.y;
      break;
    case VALIGN_BOTTOM:
      position.y -= size.y;
      break;
    default:
      TraceLog(LOG_WARNING, "bad text valign");
    }
    DrawTextEx(*label[i].font, label[i].text, position, label[i].size, 0, tinted[i].tint);
  }
  EndTextureMode();
}

//------------------------------------------------------------------------------

void render_images(ecs_iter_t *it)
{
  Renderable *renderable = ecs_column(it, Renderable, 1);
  Spatial *spatial = ecs_column(it, Spatial, 2);
  Tinted *tinted = ecs_column(it, Tinted, 3);
  RenderTexture2D *playfield = texture_manager_playfield();
  BeginTextureMode(*playfield);
  for (int i = 0; i < it->count; ++i)
  {
    Rectangle dst = {spatial[i].position.x, spatial[i].position.y, renderable[i].scale * renderable[i].src.width, renderable[i].scale * renderable[i].src.height};
    Vector2 origin = {0.5 * dst.width, 0.5 * dst.height};
    DrawTexturePro(*renderable[i].texture, renderable[i].src, dst, origin, spatial[i].rotation, tinted[i].tint);
  }
  EndTextureMode();
}
