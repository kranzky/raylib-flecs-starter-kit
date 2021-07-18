#ifndef LABEL_COMPONENT_H
#define LABEL_COMPONENT_H

#include <raylib.h>
#include <raymath.h>
#include <flecs.h>

#include "../managers/font.h"

typedef enum TextAlignment
{
  ALIGN_LEFT,
  ALIGN_CENTRE,
  ALIGN_RIGHT,
  MAX_ALIGN
} TextAlignment;

typedef enum TextVerticalAlignment
{
  VALIGN_TOP,
  VALIGN_MIDDLE,
  VALIGN_BOTTOM,
  MAX_VALIGN
} TextVerticalAlignment;

typedef struct Label
{
  Font *font;
  const char *message;
  TextAlignment align;
  TextVerticalAlignment valign;
  int size;
} Label;

ECS_COMPONENT_DECLARE(Label);

#endif
