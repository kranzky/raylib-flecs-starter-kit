#include <flecs.h>
#include <raylib.h>

#include "../components/nuklear.h"
#include "../components/window.h"
#include "../components/widget.h"

#include "../managers/font.h"

#include "nuklear.h"

//==============================================================================

static struct nk_user_font _font = {0};

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  Nuklear *nuklear = ecs_singleton_get_mut(world, Nuklear);
  nk_free(nuklear);
  ecs_singleton_modified(world, Nuklear);
}

//------------------------------------------------------------------------------

static float _nuklear_get_font_width(nk_handle handle, float size, const char *text, int length)
{
  return MeasureTextEx(*font_manager_get(FONT_CLOVER), text, size, 0.1 * size).x;
}

//------------------------------------------------------------------------------

static void _nuklear_clipboard_copy(nk_handle user, const char *text, int length)
{
  SetClipboardText(text);
}

//------------------------------------------------------------------------------

static void _nuklear_clipboard_paste(nk_handle user, struct nk_text_edit *edit)
{
  const char *text = GetClipboardText();
  if (text == NULL)
    return;
  nk_textedit_paste(edit, text, TextLength(text));
}

//------------------------------------------------------------------------------

static void _remove_window(ecs_iter_t *it)
{
  ECS_COLUMN(it, Window, window, 1);
  Nuklear *nuklear = ecs_singleton_get_mut(it->world, Nuklear);
  for (int i = 0; i < it->count; ++i)
  {
    nk_window_close(nuklear, window[i].name);
  }
  ecs_singleton_modified(it->world, Nuklear);
}

//------------------------------------------------------------------------------

void nuklear_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ecs_singleton_set(world, Nuklear, {.clip = {.copy = _nuklear_clipboard_copy, .paste = _nuklear_clipboard_paste}});
  _font = (struct nk_user_font){.height = 20, .width = _nuklear_get_font_width};
  Nuklear *nuklear = ecs_singleton_get_mut(world, Nuklear);
  nk_init_default(nuklear, &_font);
  ecs_singleton_modified(world, Nuklear);
  ECS_TRIGGER(world, _remove_window, EcsOnRemove, Window);
}

//------------------------------------------------------------------------------

ecs_entity_t nuklear_window(ecs_world_t *world, ecs_entity_t parent, const char *name, int x, int y, int width, int height)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Window, {.name = name, .bounds = (Rectangle){x, y, width, height}, .flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_CLOSABLE});
  ecs_add_pair(world, entity, EcsChildOf, parent);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t nuklear_label(ecs_world_t *world, ecs_entity_t window, const char *name)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Widget, {.type = WIDGET_LABEL, .name = name});
  ecs_add_pair(world, entity, EcsChildOf, window);
  return entity;
}
