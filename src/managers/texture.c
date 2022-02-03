#include "../defines.h"

#include "texture.h"

//==============================================================================

static Texture _textures[MAX_TEXTURES];
static RenderTexture2D _playfield;

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  for (int i = 0; i < MAX_TEXTURES; ++i)
    UnloadTexture(_textures[i]);
  UnloadRenderTexture(_playfield);
}

//------------------------------------------------------------------------------

static void _load(int id, const char *name)
{
  char filename[256];
  sprintf(filename, "./res/gfx/%s.png", name);
  _textures[id] = LoadTexture(filename);
}

//------------------------------------------------------------------------------

void texture_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  _playfield = LoadRenderTexture(RASTER_WIDTH, RASTER_HEIGHT);
  _load(TEXTURE_LOGO, "logo");
  _load(TEXTURE_SHIP, "ship");
  _load(TEXTURE_BULLET, "bullet");
  _load(TEXTURE_SHEEP, "sheep");
  _load(TEXTURE_BLIP, "blip");
}

//------------------------------------------------------------------------------

Texture *texture_manager_get(TextureName id)
{
  return (id == MAX_TEXTURES) ? NULL : &_textures[id];
}
//------------------------------------------------------------------------------

RenderTexture2D *texture_manager_playfield()
{
  return &_playfield;
}
