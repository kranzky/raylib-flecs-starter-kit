#include <raylib.h>

#include "../components/nuklear.h"

#include "nuklear.h"

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
}

//------------------------------------------------------------------------------

static float _nuklear_get_font_width(nk_handle handle, float height, const char *text, int length)
{
  return MeasureText(text, height);
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

void nuklear_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ecs_singleton_set(world, Nuklear, {.clip = {.copy = _nuklear_clipboard_copy, .paste = _nuklear_clipboard_paste}});
  Nuklear *nuklear = ecs_singleton_get_mut(world, Nuklear);
  nk_init_default(nuklear, NULL);
  ecs_singleton_modified(world, Nuklear);
}
