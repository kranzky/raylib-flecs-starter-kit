#include <raylib.h>

#include "../components/input.h"
#include "../components/nuklear.h"
#include "../components/window.h"

#include "../managers/texture.h"
#include "../managers/font.h"

#include "nuklear.h"

//==============================================================================

void nuklear_input(ecs_iter_t *it)
{
  Nuklear *nuklear = ecs_column(it, Nuklear, 1);
  Input *input = ecs_column(it, Input, 2);
  nk_input_begin(nuklear);
  // nk_input_motion(nuklear, x, y);
  // NK_BUTTON_LEFT
  // nk_input_button(nuklear, btn, x, y, down);
  // NK_KEY_UP NK_KEY_DOWN NK_KEY_LEFT NK_KEY_RIGHT NK_KEY_ENTER NK_KEY_TAB NK_KEY_BACKSPACE
  // nk_input_key(nuklear, key, down);
  // nk_input_unicode(nuklear, rune);
  nk_input_end(nuklear);
}

//------------------------------------------------------------------------------

static inline struct nk_rect _to_rect(Rectangle rect)
{
  return nk_rect(rect.x, rect.y, rect.width, rect.height);
}

//------------------------------------------------------------------------------

static inline Color _from_color(struct nk_color color)
{
  return (Color){color.r, color.g, color.b, color.a};
}

//------------------------------------------------------------------------------

void nuklear_update(ecs_iter_t *it)
{
  Nuklear *nuklear = ecs_column(it, Nuklear, 1);
  Window *window = ecs_column(it, Window, 2);
  for (int i = 0; i < it->count; ++i)
  {
    if (nk_begin(nuklear, window[i].name, _to_rect(window[i].bounds), window[i].flags))
    {
      // TODO: child rows
    }
    nk_end(nuklear);
  }
}

//------------------------------------------------------------------------------

static inline void _scissor(const struct nk_command_scissor *command)
{
  BeginScissorMode(command->x, command->y, command->w, command->h);
}

//------------------------------------------------------------------------------

static inline void _line(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: line");
}

//------------------------------------------------------------------------------

static inline void _curve(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: curve");
}

//------------------------------------------------------------------------------

static inline void _rect(const struct nk_command_rect *command)
{
  Color color = _from_color(command->color);
  Rectangle rect = (Rectangle){command->x, command->y, command->w, command->h};
  if (command->rounding > 0)
    DrawRectangleRoundedLines(rect, command->rounding * 0.05, 1, command->line_thickness, color);
  else
    DrawRectangleLinesEx(rect, command->line_thickness, color);
}

//------------------------------------------------------------------------------

static inline void _rect_filled(const struct nk_command_rect_filled *command)
{
  Color color = _from_color(command->color);
  Rectangle rect = (Rectangle){command->x, command->y, command->w, command->h};
  if (command->rounding > 0)
    DrawRectangleRounded(rect, command->rounding * 0.05, 1, color);
  else
    DrawRectangleRec(rect, color);
}

//------------------------------------------------------------------------------

static inline void _rect_multi_color(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: rect_multi_color");
}

//------------------------------------------------------------------------------

static inline void _circle(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: circle");
}

//------------------------------------------------------------------------------

static inline void _circle_filled(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: circle_filled");
}

//------------------------------------------------------------------------------

static inline void _arc(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: arc");
}

//------------------------------------------------------------------------------

static inline void _arc_filled(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: arc_filled");
}

//------------------------------------------------------------------------------

static inline void _triangle(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: triangle");
}

//------------------------------------------------------------------------------

static inline void _triangle_filled(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: triangle_filled");
}

//------------------------------------------------------------------------------

static inline void _polygon(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: polygon");
}

//------------------------------------------------------------------------------

static inline void _polygon_filled(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: polygon_filled");
}

//------------------------------------------------------------------------------

static inline void _polyline(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: polyline");
}

//------------------------------------------------------------------------------

static inline void _text(const struct nk_command_text *command)
{
  Color fore = _from_color(command->foreground);
  Color back = _from_color(command->background);
  float size = command->font->height;
  Rectangle rect = (Rectangle){command->x, command->y, command->w, command->h};
  DrawRectangleRec(rect, back);
  Vector2 position = (Vector2){command->x, command->y};
  DrawTextEx(*font_manager_get(FONT_CLOVER), command->string, (Vector2){command->x, command->y}, size, 0.1 * size, fore);
}

//------------------------------------------------------------------------------

static inline void _image(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: image");
}

//------------------------------------------------------------------------------

static inline void _custom(const struct nk_command *command)
{
  TraceLog(LOG_WARNING, "Unimplemented Nuklear Command: custom");
}

//------------------------------------------------------------------------------

void nuklear_render(ecs_iter_t *it)
{
  Nuklear *nuklear = ecs_column(it, Nuklear, 1);
  const struct nk_command *command;
  BeginTextureMode(*texture_manager_playfield());
  nk_foreach(command, nuklear)
  {
    switch (command->type)
    {
    case NK_COMMAND_NOP:
      break;
    case NK_COMMAND_SCISSOR:
      _scissor((const struct nk_command_scissor *)command);
      break;
    case NK_COMMAND_LINE:
      _line(command);
      break;
    case NK_COMMAND_CURVE:
      _curve(command);
      break;
    case NK_COMMAND_RECT:
      _rect((const struct nk_command_rect *)command);
      break;
    case NK_COMMAND_RECT_FILLED:
      _rect_filled((const struct nk_command_rect_filled *)command);
      break;
    case NK_COMMAND_RECT_MULTI_COLOR:
      _rect_multi_color(command);
      break;
    case NK_COMMAND_CIRCLE:
      _circle(command);
      break;
    case NK_COMMAND_CIRCLE_FILLED:
      _circle_filled(command);
      break;
    case NK_COMMAND_ARC:
      _arc(command);
      break;
    case NK_COMMAND_ARC_FILLED:
      _arc_filled(command);
      break;
    case NK_COMMAND_TRIANGLE:
      _triangle(command);
      break;
    case NK_COMMAND_TRIANGLE_FILLED:
      _triangle_filled(command);
      break;
    case NK_COMMAND_POLYGON:
      _polygon(command);
      break;
    case NK_COMMAND_POLYGON_FILLED:
      _polygon_filled(command);
      break;
    case NK_COMMAND_POLYLINE:
      _polyline(command);
      break;
    case NK_COMMAND_TEXT:
      _text((const struct nk_command_text *)command);
      break;
    case NK_COMMAND_IMAGE:
      _image(command);
      break;
    case NK_COMMAND_CUSTOM:
      _custom(command);
      break;
    }
  }
  EndTextureMode();
  nk_clear(nuklear);
}
