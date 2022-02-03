#include <raylib.h>

#include "../helpers.h"

#include "../components/input.h"
#include "../components/interface.h"
#include "../components/window.h"
#include "../components/widget.h"
#include "../components/settings.h"
#include "../components/spatial.h"

#include "../managers/texture.h"
#include "../managers/font.h"
#include "../managers/entity.h"
#include "../managers/sound.h"

#include "gui.h"

//==============================================================================

static bool _scissoring = false;
static Vector2 _pointer = {0.5 * RASTER_WIDTH, 267.5};
static Vector2 _mouse = {0.5 * RASTER_WIDTH, 0.5 * RASTER_HEIGHT};
static bool _mode = true;
static ecs_entity_t _hovered = 0;
static float _timer = 0;

//==============================================================================

void gui_input(ecs_iter_t *it)
{
  Interface *interface = ecs_term(it, Interface, 1);
  Input *input = ecs_term(it, Input, 2);
  Window *window = ecs_term(it, Window, 4);
  _pointer.x = 0.5 * RASTER_WIDTH;
  unsigned int max = 0;
  for (int i = 0; i < it->count; ++i)
  {
    if (window[i].max > max)
      max = window[i].max;
  }
  max -= 1;
  if (_timer > 0)
  {
    if (input->joystick.y < -0.1)
    {
      _pointer.y -= 100;
      _timer = -0.15;
      _mode = false;
    }
    if (input->joystick.y > 0.1)
    {
      _pointer.y += 100;
      _timer = -0.15;
      _mode = false;
    }
  }
  _timer += it->delta_time;
  _pointer.y = Clamp(_pointer.y, 350, 350 + max * 100);
  Vector2 delta = Vector2Subtract(_mouse, input->pointer);
  _mouse = input->pointer;
  if (Vector2LengthSqr(delta) > 0.1 || _mode)
  {
    _pointer = _mouse;
    ShowCursor();
    _mode = true;
  }
  else if (!_mode)
  {
    HideCursor();
  }
  nk_input_begin(interface);
  nk_input_motion(interface, _pointer.x, _pointer.y);
  nk_input_scroll(interface, (struct nk_vec2){0, input->wheel});
  nk_input_button(interface, NK_BUTTON_LEFT, _pointer.x, _pointer.y, input->select);
  nk_input_end(interface);
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

void gui_update(ecs_iter_t *it)
{
  Interface *interface = ecs_term(it, Interface, 1);
  Window *window = ecs_term(it, Window, 2);
  Widget *widget = ecs_term(it, Widget, 3);
  bool hover = false;
  if (nk_begin(interface, window->name, _to_rect(window->bounds), window->flags))
  {
    for (int i = 0; i < it->count; ++i)
    {
      if (i == 0 || widget[i].type == WIDGET_SEPARATOR)
      {
        int count = (i == 0) ? 1 : 0;
        for (int j = i + 1; j < it->count; ++j, ++count)
          if (widget[j].type == WIDGET_SEPARATOR)
            break;
        nk_layout_row_static(interface, 96, 1200, 1);
        if (nk_widget_is_hovered(interface))
        {
          hover = true;
          _hovered = it->entities[i];
        }
      }
      if (nk_widget_is_hovered(interface))
      {
        hover = true;
        _hovered = it->entities[i];
      }
      switch (widget[i].type)
      {
      case WIDGET_LABEL:
        nk_label(interface, widget[i].name, NK_TEXT_CENTERED);
        break;
      case WIDGET_BUTTON:
        if (nk_button_label(interface, widget[i].name))
          widget[i].callback(it->world, &widget[i]);
        break;
      case WIDGET_SLIDER:
        if (nk_slider_float(interface, 0, &widget[i].value, 100, 0.1))
          widget[i].callback(it->world, &widget[i]);
        break;
      }
    }
  }
  nk_end(interface);
  if (!hover)
    _hovered = 0;
}

//------------------------------------------------------------------------------

static inline void _scissor(const struct nk_command_scissor *command)
{
  if (_scissoring)
    EndScissorMode();
  BeginScissorMode(command->x, command->y, command->w, command->h);
  _scissoring = true;
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

static inline void _circle_filled(const struct nk_command_circle_filled *command)
{
  Color color = _from_color(command->color);
  DrawEllipse(command->x + 0.5 * command->w, command->y + 0.5 * command->h, 0.5 * command->w, 0.5 * command->h, color);
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

static inline void _triangle_filled(const struct nk_command_triangle_filled *command)
{
  Color color = _from_color(command->color);
  DrawTriangle((Vector2){command->b.x, command->b.y}, (Vector2){command->a.x, command->a.y}, (Vector2){command->c.x, command->c.y}, color);
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
  DrawTextEx(*font_manager_get(FONT_CLOVER), command->string, position, size, 0, fore);
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

void gui_render(ecs_iter_t *it)
{
  Interface *interface = ecs_term(it, Interface, 1);
  const struct nk_command *command;
  _scissoring = false;
  BeginTextureMode(*texture_manager_playfield());
  nk_foreach(command, interface)
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
      _circle_filled((const struct nk_command_circle_filled *)command);
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
      _triangle_filled((struct nk_command_triangle_filled *)command);
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
  if (_scissoring)
    EndScissorMode();
  EndTextureMode();
  nk_clear(interface);
}

//------------------------------------------------------------------------------

void gui_reset(int row)
{
  _pointer.y = 267.5 + row * 100;
  _hovered = 0;
  _mode = false;
}

//------------------------------------------------------------------------------

void update_console(ecs_iter_t *it)
{
  Spatial *spatial = ecs_term(it, Spatial, 1);
  for (int i = 0; i < it->count; ++i)
  {
    spatial[i].position.y = RASTER_HEIGHT * 0.75 - (it->count - i - 1) * 90;
  }
}
