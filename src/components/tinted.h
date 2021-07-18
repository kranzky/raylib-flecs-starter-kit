#ifndef TINTED_COMPONENT_H
#define TINTED_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

typedef struct Tinted
{
  Color tint;
} Tinted;

ECS_COMPONENT_DECLARE(Tinted);

#endif
