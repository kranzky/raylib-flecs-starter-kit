#include "../components/label.h"
#include "../components/spatial.h"
#include "../components/tinted.h"

#include "../managers/texture.h"

//==============================================================================

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
