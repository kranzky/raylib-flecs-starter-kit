#include "../defines.h"
#include "../helpers.h"

#include "../components/label.h"
#include "../components/spatial.h"
#include "../components/tinted.h"
#include "../components/renderable.h"
#include "../components/scene.h"
#include "../components/settings.h"
#include "../components/transition.h"
#include "../components/display.h"
#include "../components/viewport.h"
#include "../components/physical.h"
#include "../components/animated.h"
#include "../components/stateful.h"
#include "../components/aligned.h"
#include "../components/time.h"

#include "../managers/texture.h"
#include "../managers/system.h"

#include "render.h"

//==============================================================================

void refresh_display(ecs_iter_t *it)
{
  Display *display = ecs_term(it, Display, 1);

  display->window.width = GetScreenWidth();
  display->window.height = GetScreenHeight();
#ifdef MAC
  display->window.width *= GetWindowScaleDPI().x;
  display->window.height *= GetWindowScaleDPI().y;
#endif

  display->scale = MIN(display->window.width / display->raster.width, display->window.height / display->raster.height);

  display->screen = (Rectangle){
      (display->window.width - (display->raster.width * display->scale)) * 0.5f,
      (display->window.height - (display->raster.height * display->scale)) * 0.5f,
      display->raster.width * display->scale,
      display->raster.height * display->scale};
}

//------------------------------------------------------------------------------

void render_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_term(it, Scene, 1);
  Stateful *stateful = ecs_term(it, Stateful, 2);
  Transition *transition = ecs_term(it, Transition, 3);
  Display *display = ecs_term(it, Display, 4);
  Time *time = ecs_term(it, Time, 5);
  RenderTexture2D *playfield = texture_manager_playfield();
  BeginTextureMode(*playfield);
  for (int i = 0; i < it->count; ++i)
  {
    switch (stateful[i].state)
    {
    case STATE_STARTING:
    case STATE_RUNNING:
    case STATE_STOPPING:
    {
      if (time->paused)
      {
        display->background.a = 255;
      }
      else
      {
        display->background.a = (int)(255.0 * transition[i].fade);
      }
      if (scene[i].shader != NULL)
      {
        BeginShaderMode(*scene[i].shader);
        SetShaderValue(*scene[i].shader, 0, &scene[i].time, SHADER_UNIFORM_FLOAT);
        DrawTextureEx(playfield->texture, (Vector2){0}, 0, 1, scene[i].color);
        EndShaderMode();
      }
      else if (scene[i].texture != NULL)
      {
        Rectangle src = {0, 0, scene[i].texture->width, scene[i].texture->height};
        Rectangle dst = {0, 0, RASTER_WIDTH, RASTER_HEIGHT};
        Vector2 origin = {0, 0};
        DrawTexturePro(*scene[i].texture, src, dst, origin, 0, WHITE);
      }
      else
      {
        ClearBackground(scene[i].color);
      }
      break;
    }
    }
  }
  EndTextureMode();
}

//------------------------------------------------------------------------------

static inline Vector2 _align(Vector2 position, Vector2 size, Aligned aligned)
{
  switch (aligned.align)
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
    TraceLog(LOG_WARNING, "bad align");
  }
  switch (aligned.valign)
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
    TraceLog(LOG_WARNING, "bad valign");
  }
  return position;
}

//------------------------------------------------------------------------------

void render_labels(ecs_iter_t *it)
{
  Time *time = ecs_term(it, Time, 1);
  Label *label = ecs_term(it, Label, 2);
  Aligned *aligned = ecs_term(it, Aligned, 3);
  Spatial *spatial = ecs_term(it, Spatial, 4);
  Tinted *tinted = ecs_term(it, Tinted, 5);
  RenderTexture2D *playfield = texture_manager_playfield();
  BeginTextureMode(*playfield);
  if (time->paused)
  {
    const Texture *texture = texture_manager_get(TEXTURE_BLIP);
    Rectangle src = {0, 0, 3, 3};
    Rectangle dst = {0, 0, RASTER_WIDTH, RASTER_HEIGHT};
    DrawTexturePro(*texture, src, dst, (Vector2){0, 0}, 0, (Color){0, 0, 0, 64});
    dst.y = RASTER_HEIGHT * 0.5 - 70;
    dst.height = 120;
    DrawTexturePro(*texture, src, dst, (Vector2){0, 0}, 0, ORANGE);
  }
  for (int i = 0; i < it->count; ++i)
  {
    Vector2 size = MeasureTextEx(*label[i].font, label[i].text, label[i].size, 0);
    Vector2 position = _align(spatial[i].position, size, aligned[i]);
    DrawTextEx(*label[i].font, label[i].text, position, label[i].size, 0, tinted[i].tint);
  }
  EndTextureMode();
}

//------------------------------------------------------------------------------

void render_images(ecs_iter_t *it)
{
  Renderable *renderable = ecs_term(it, Renderable, 1);
  Spatial *spatial = ecs_term(it, Spatial, 2);
  Tinted *tinted = ecs_term(it, Tinted, 3);
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

//------------------------------------------------------------------------------

static inline void _render_physical(ecs_iter_t *it)
{
  Renderable *renderable = ecs_term(it, Renderable, 1);
  Physical *physical = ecs_term(it, Physical, 2);
  Tinted *tinted = ecs_term(it, Tinted, 3);
  Transition *transition = ecs_term(it, Transition, 4);
  for (int i = 0; i < it->count; ++i)
  {
    if (physical[i].body->space == NULL)
      continue;
    Color tint = tinted[i].tint;
    int alpha = tint.a;
    tint.a = 192;
    Vector2 prev = Vector2Zero();
    bool rope = false;
    for (int j = 0; j < 9; ++j)
    {
      if (physical[i].joints[j].body == NULL)
        continue;
      rope = true;
      Vector2 next = _to_vector(cpBodyGetPosition(physical[i].joints[j].body));
      if (j > 0)
        DrawLineEx(prev, next, 0.2, tint);
      prev = next;
    }
    Vector2 position = _to_vector(cpBodyGetPosition(physical[i].body));
    if (rope)
      DrawLineEx(prev, position, 0.2, tint);
    tint.a = alpha;
    if (transition)
      tint.a = (int)(255 * transition[i].fade);
    float angle = RAD2DEG * cpBodyGetAngle(physical[i].body) + 180;
    Rectangle dst = {position.x, position.y, renderable[i].src.width * renderable[i].scale, renderable[i].src.height * renderable[i].scale};
    Vector2 origin = {0.5 * dst.width, 0.5 * dst.height};
    BeginBlendMode(renderable[i].blend_mode);
    DrawTexturePro(*renderable[i].texture, renderable[i].src, dst, origin, angle, tint);
    EndBlendMode();
  }
}

//------------------------------------------------------------------------------

void render_physical(ecs_iter_t *it)
{
  ecs_iter_t vit = ecs_query_iter(system_manager_viewport_query());
  while (ecs_query_next(&vit))
  {
    Viewport *viewport = ecs_term(&vit, Viewport, 1);
    for (int i = 0; i < vit.count; ++i)
    {
      if (!viewport[i].active)
        continue;
      BeginTextureMode(viewport[i].raster);
      BeginMode2D(viewport[i].camera);
      _render_physical(it);
      EndMode2D();
      EndTextureMode();
    }
  }
}

//------------------------------------------------------------------------------

void render_viewports(ecs_iter_t *it)
{
  Viewport *viewport = ecs_term(it, Viewport, 1);
  RenderTexture2D *playfield = texture_manager_playfield();
  Font *font = font_manager_get(FONT_CLOVER);
  Vector2 size = MeasureTextEx(*font, "Connect Controller", 48, 0);
  const Texture *texture = texture_manager_get(TEXTURE_BLIP);
  BeginTextureMode(*playfield);
  for (int i = 0; i < it->count; ++i)
  {
    Rectangle dst = viewport[i].dst;
    dst.x += viewport[i].origin.x;
    dst.y += viewport[i].origin.y;
    DrawTexturePro(viewport[i].raster.texture, viewport[i].src, dst, viewport[i].origin, viewport[i].rotation, viewport[i].color);
    if (viewport[i].active)
      continue;
    DrawTexturePro(*texture, (Rectangle){0, 0, 3, 3}, (Rectangle){dst.x - dst.width / 2, dst.y - size.y / 2 - 7, dst.width, size.y + 10}, (Vector2){0, 0}, 0, (Color){255, 0, 255, 255});
    Vector2 position = {dst.x - size.x / 2, dst.y - size.y / 2};
    DrawTextEx(*font, "Connect Controller", position, 48, 0, (Color){0, 255, 255, 255});
  }
  EndTextureMode();
}

//------------------------------------------------------------------------------

void composite_display(ecs_iter_t *it)
{
  Display *display = ecs_term(it, Display, 1);
  RenderTexture2D *playfield = texture_manager_playfield();
  BeginDrawing();
  ClearBackground(display->border);
  Rectangle src = display->raster;
  src.height *= -1;
  DrawTexturePro(playfield->texture, src, display->screen, Vector2Zero(), 0, display->background);
  EndDrawing();
}

//------------------------------------------------------------------------------

void animate(ecs_iter_t *it)
{
  Animated *animated = ecs_column(it, Animated, 1);
  Renderable *renderable = ecs_column(it, Renderable, 2);

  for (int i = 0; i < it->count; ++i)
  {
    int frame = (animated[i].frames * animated[i].time) / animated[i].duration;
    if (frame >= animated[i].frames)
    {
      if (animated[i].loop)
      {
        animated[i].time = 0;
        frame = 0;
      }
      else
      {
        ecs_delete(it->world, it->entities[i]);
        continue;
      }
    }
    animated[i].time += it->delta_time;
    int x = (frame + animated[i].begin) * animated[i].width;
    int y = (x / renderable[i].texture->width) * animated[i].height;
    x %= renderable[i].texture->width;
    renderable[i].src = (Rectangle){x, y, animated[i].width, animated[i].height};
  }
}
