#include <raylib.h>
#include <raymath.h>

#include "../components/input.h"
#include "../components/settings.h"
#include "../components/display.h"

#include "input.h"

#include "../defines.h"

//==============================================================================

void process_input(ecs_iter_t *it)
{
  Input *input = ecs_column(it, Input, 1);
  Settings *settings = ecs_column(it, Settings, 2);
  Display *display = ecs_column(it, Display, 3);
  *input = (Input){0};

  input->quit |= WindowShouldClose();
  input->quit |= IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q);
  input->quit |= IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_W);
  input->toggle_fullscreen = IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_F);
  input->toggle_pause = IsKeyPressed(KEY_P);

  if (settings->gamepad == -1)
  {
    for (int i = 0; i < 4; ++i)
      if (IsGamepadAvailable(i) && IsGamepadButtonDown(i, GAMEPAD_BUTTON_MIDDLE))
        settings->gamepad = i;
  }
  else
  {
    input->quit |= IsGamepadButtonPressed(settings->gamepad, GAMEPAD_BUTTON_MIDDLE_LEFT);
    input->select |= IsGamepadButtonPressed(settings->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    input->select |= IsGamepadButtonPressed(settings->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
    input->toggle_pause |= IsGamepadButtonPressed(settings->gamepad, GAMEPAD_BUTTON_MIDDLE_RIGHT);
    input->fire |= IsGamepadButtonDown(settings->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    input->fire |= IsGamepadButtonDown(settings->gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
    input->fire |= IsGamepadButtonDown(settings->gamepad, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
    input->fire |= IsGamepadButtonDown(settings->gamepad, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
    input->joystick.x = GetGamepadAxisMovement(settings->gamepad, GAMEPAD_AXIS_LEFT_X);
    input->joystick.y = GetGamepadAxisMovement(settings->gamepad, GAMEPAD_AXIS_LEFT_Y);
    if (IsGamepadButtonPressed(settings->gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
      input->joystick.x = -1;
    if (IsGamepadButtonPressed(settings->gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
      input->joystick.x = 1;
    if (IsGamepadButtonPressed(settings->gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP))
      input->joystick.y = -1;
    if (IsGamepadButtonPressed(settings->gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
      input->joystick.y = 1;
  }

  input->fire |= IsMouseButtonDown(MOUSE_LEFT_BUTTON);
  input->select |= IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
  input->pointer = Vector2Scale(Vector2Subtract(GetMousePosition(), (Vector2){display->screen.x, display->screen.y}), 1.0 / display->scale);
  input->wheel = GetMouseWheelMove();

  input->fire |= IsKeyDown(KEY_ENTER);
  input->fire |= IsKeyDown(KEY_SPACE);
  input->select |= IsKeyPressed(KEY_ENTER);
  input->select |= IsKeyPressed(KEY_SPACE);
  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_J) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_KP_4))
    input->joystick.x = -1;
  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_L) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_KP_6))
    input->joystick.x = 1;
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_I) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_KP_8))
    input->joystick.y = -1;
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_K) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_KP_2))
    input->joystick.y = 1;

  if (Vector2LengthSqr(input->joystick) < 0.1)
    input->joystick = Vector2Zero();

#ifdef DEBUG
  input->toggle_debug = IsKeyPressed(KEY_TAB);
#endif
}
