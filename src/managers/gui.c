#include <flecs.h>
#include <raylib.h>

#include "../components/interface.h"
#include "../components/window.h"
#include "../components/widget.h"

#include "../managers/font.h"

#include "../systems/gui.h"

#include "gui.h"

//==============================================================================

static struct nk_user_font _font = {0};

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  Interface *interface = ecs_singleton_get_mut(world, Interface);
  nk_free(interface);
  ecs_singleton_modified(world, Interface);
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
  Window *window = ecs_term(it, Window, 1);
  Interface *interface = ecs_singleton_get_mut(it->world, Interface);
  for (int i = 0; i < it->count; ++i)
  {
    nk_window_close(interface, window[i].name);
  }
  ecs_singleton_modified(it->world, Interface);
}

//------------------------------------------------------------------------------

void gui_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ecs_singleton_set(world, Interface, {.clip = {.copy = _nuklear_clipboard_copy, .paste = _nuklear_clipboard_paste}});
  _font = (struct nk_user_font){.height = 60u, .width = _nuklear_get_font_width};
  Interface *interface = ecs_singleton_get_mut(world, Interface);
  nk_init_default(interface, &_font);
  interface->style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 128));
  interface->style.button.normal = nk_style_item_color(nk_rgba(0, 0, 0, 0));
  interface->style.button.hover = nk_style_item_color(nk_rgba(255, 0, 255, 224));
  interface->style.button.active = nk_style_item_color(nk_rgba(0, 255, 255, 255));
  interface->style.button.border_color = nk_rgba(0, 0, 0, 0);
  interface->style.button.text_normal = nk_rgba(255, 255, 255, 224);
  interface->style.button.text_hover = nk_rgba(0, 255, 255, 224);
  interface->style.button.text_active = nk_rgba(255, 0, 255, 255);
  ecs_singleton_modified(world, Interface);
  ECS_TRIGGER(world, _remove_window, EcsOnRemove, Window);
}

//------------------------------------------------------------------------------

ecs_entity_t gui_window(ecs_world_t *world, ecs_entity_t parent, const char *name, int x, int y, int width, int height, int row, int max)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Window, {.name = name, .bounds = (Rectangle){x, y, width, height}, .flags = NK_WINDOW_NO_SCROLLBAR, .max = max});
  ecs_add_pair(world, entity, EcsChildOf, parent);
  gui_reset(row);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t gui_label(ecs_world_t *world, ecs_entity_t window, const char *name)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Widget, {.type = WIDGET_LABEL, .name = name});
  ecs_add_pair(world, entity, EcsChildOf, window);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t gui_button(ecs_world_t *world, ecs_entity_t window, const char *name, int value, widgetCallback callback)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Widget, {.type = WIDGET_BUTTON, .name = name, .value = value, .callback = callback});
  ecs_add_pair(world, entity, EcsChildOf, window);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t gui_slider(ecs_world_t *world, ecs_entity_t window, float value, widgetCallback callback)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Widget, {.type = WIDGET_SLIDER, .value = value, .callback = callback});
  ecs_add_pair(world, entity, EcsChildOf, window);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t gui_separator(ecs_world_t *world, ecs_entity_t window)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Widget, {.type = WIDGET_SEPARATOR});
  ecs_add_pair(world, entity, EcsChildOf, window);
  return entity;
}
