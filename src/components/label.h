#ifndef LABEL_COMPONENT_H
#define LABEL_COMPONENT_H

#include <raylib.h>
#include <flecs.h>

#include "../managers/font.h"

typedef struct Label
{
  Font *font;
  const char *text;
  int size;
} Label;

ECS_COMPONENT_DECLARE(Label);

#endif
