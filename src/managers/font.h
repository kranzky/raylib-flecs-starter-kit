#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <flecs.h>
#include <raylib.h>

typedef enum FontName
{
  FONT_CLOVER,
  MAX_FONTS
} FontName;

void font_manager_init(ecs_world_t *world);
Font *font_manager_get(FontName id);

#endif
