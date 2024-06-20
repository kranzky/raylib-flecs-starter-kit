#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <flecs.h>
#include <raylib.h>

typedef enum TextureName
{
  TEXTURE_LOGO,
  TEXTURE_SHIP,
  TEXTURE_BULLET,
  TEXTURE_SHEEP,
  TEXTURE_BLIP,
  MAX_TEXTURES
} TextureName;

void texture_manager_init(ecs_world_t *world);
Texture *texture_manager_get(TextureName id);
RenderTexture2D *texture_manager_playfield(void);

#endif
