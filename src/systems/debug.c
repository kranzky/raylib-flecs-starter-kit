#ifdef DEBUG

#include <raylib.h>

#include "../defines.h"

#include "../managers/texture.h"
#include "../managers/entity.h"
#include "../managers/physics.h"

#include "../components/debug.h"
#include "../components/input.h"
#include "../components/scene.h"
#include "../components/physical.h"
#include "../components/viewport.h"
#include "../components/display.h"
#include "../components/stateful.h"
#include "../components/audible.h"
#include "../components/transition.h"

#include "debug.h"

//==============================================================================

static const char *_scene_name[] = {
    [SCENE_SPLASH] = "Splash screen",
    [SCENE_TITLE] = "Main menu",
    [SCENE_LEVEL] = "Game level",
    [SCENE_GAME_OVER] = "Game over"};

static const char *_scene_state[] = {
    [STATE_CREATED] = "created",
    [STATE_STARTING] = "starting",
    [STATE_RUNNING] = "running",
    [STATE_STOPPING] = "stopping",
    [STATE_STOPPED] = "stopped"};

static char _buffer[4096] = {0};
static char *_pointer = _buffer;

//==============================================================================

void _debug_text(ecs_world_t *world, const char *format, ...)
{
  va_list argptr;
  va_start(argptr, format);
  vsprintf(_pointer, format, argptr);
  va_end(argptr);
  entity_manager_spawn_debug(world, _pointer);
  _pointer += strlen(_pointer) + 1;
}

//------------------------------------------------------------------------------

void debug_input(ecs_iter_t *it)
{
  Input *input = ecs_term(it, Input, 1);
  Debug *debug = ecs_term(it, Debug, 2);
  if (input->toggle_debug)
    debug->enabled = !debug->enabled;
  if (!debug->enabled)
    return;
  _debug_text(it->world, "Buttons: %s %s", input->select ? "SELECT" : "select", input->quit ? "QUIT" : "quit");
  _debug_text(it->world, "Stick: %3.2f, %3.2f", input->joystick.x, input->joystick.y);
  if (Vector2LengthSqr(input->joystick) > 0.5)
  {
    float angle = RAD2DEG * atan2f(-input->joystick.x, -input->joystick.y);
    _debug_text(it->world, "Angle: %3.2f", angle);
  }
  _debug_text(it->world, "Mouse: %3.2f, %3.2f", input->pointer.x, input->pointer.y);
}

//------------------------------------------------------------------------------

void debug_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_term(it, Scene, 1);
  Stateful *stateful = ecs_term(it, Stateful, 2);
  Debug *debug = ecs_term(it, Debug, 3);
  if (!debug->enabled)
    return;
  for (int i = 0; i < it->count; ++i)
  {
    _debug_text(it->world, "%s is %s.", _scene_name[scene[i].id], _scene_state[stateful[i].state]);
  }
  _debug_text(it->world, "Physical Components: %d", ecs_count(it->world, Physical));
  _debug_text(it->world, "Audible Components: %d", ecs_count(it->world, Audible));
  _debug_text(it->world, "Stateful Components: %d", ecs_count(it->world, Stateful));
  _debug_text(it->world, "Transition Components: %d", ecs_count(it->world, Transition));
}

//------------------------------------------------------------------------------

void debug_render(ecs_iter_t *it)
{
  Label *label = ecs_term(it, Label, 1);
  Debug *debug = ecs_term(it, Debug, 2);
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

//------------------------------------------------------------------------------

void debug_physics(ecs_iter_t *it)
{
  Physics *physics = ecs_term(it, Physics, 1);
  Debug *debug = ecs_term(it, Debug, 2);
  Viewport *viewport = ecs_term(it, Viewport, 3);
  if (!debug->enabled)
    return;
  for (int i = 0; i < it->count; ++i)
  {
    if (!viewport[i].active)
      continue;
    BeginTextureMode(viewport[i].raster);
    BeginMode2D(viewport[i].camera);
    cpSpaceDebugDraw(physics->space, physics_debug_options());
    EndMode2D();
    EndTextureMode();
  }
}

//------------------------------------------------------------------------------

void debug_display(ecs_iter_t *it)
{
  Display *display = ecs_term(it, Display, 1);
  Debug *debug = ecs_term(it, Debug, 2);
  if (!debug->enabled)
    return;
  _debug_text(it->world, "Display: %3.2f, %3.2f; Scale: %1.3f", display->window.width, display->window.height, display->scale);
}

#else
static int _debug_disabled = 0;
#endif
