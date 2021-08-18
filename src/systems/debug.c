#include <raylib.h>

#include "../defines.h"

#include "../managers/texture.h"
#include "../managers/entity.h"

#include "../components/debug.h"
#include "../components/input.h"
#include "../components/scene.h"

#include "debug.h"

//==============================================================================

static const char *_scene_name[] = {
    [SCENE_SPLASH] = "Splash screen",
    [SCENE_TITLE] = "Main menu",
    [SCENE_LEVEL] = "Game level",
    [SCENE_GAME_OVER] = "Game over"};

static const char *_scene_state[] = {
    [SCENE_STATE_STARTING] = "starting",
    [SCENE_STATE_RUNNING] = "running",
    [SCENE_STATE_STOPPING] = "stopping"};

static char _buffer[4096] = {0};
static char *_pointer = _buffer;

//==============================================================================

void _debug_text(const char *format, ...)
{
  va_list argptr;
  va_start(argptr, format);
  vsprintf(_pointer, format, argptr);
  va_end(argptr);
  entity_manager_spawn_debug(_pointer);
  _pointer += strlen(_pointer) + 1;
}

//------------------------------------------------------------------------------

void debug_input(ecs_iter_t *it)
{
  Input *input = ecs_column(it, Input, 1);
  Debug *debug = ecs_column(it, Debug, 2);
  if (input->toggle_debug)
    debug->enabled = !debug->enabled;
  if (!debug->enabled)
    return;
  _debug_text("Buttons: %s %s %s", input->select ? "SELECT" : "select", input->fire ? "FIRE" : "fire", input->quit ? "QUIT" : "quit");
  _debug_text("Stick: %3.2f, %3.2f", input->joystick.x, input->joystick.y);
  _debug_text("Mouse: %3.2f, %3.2f", input->pointer.x, input->pointer.y);
}

//------------------------------------------------------------------------------

void debug_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_column(it, Scene, 1);
  Debug *debug = ecs_column(it, Debug, 2);
  if (!debug->enabled)
    return;
  for (int i = 0; i < it->count; ++i)
  {
    _debug_text("%s is %s.", _scene_name[scene[i].id], _scene_state[scene[i].state]);
  }
}

//------------------------------------------------------------------------------

void debug_render(ecs_iter_t *it)
{
  Label *label = ecs_column(it, Label, 1);
  Debug *debug = ecs_column(it, Debug, 2);
  _pointer = _buffer;
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
  Vector2 position = {10, 10};
  for (int i = 0; i < it->count; ++i)
  {
    DrawTextEx(GetFontDefault(), label[i].text, position, DEBUG_SIZE, 4, RED);
    position.y += height;
    ecs_delete(it->world, it->entities[i]);
  }
  EndTextureMode();
}
