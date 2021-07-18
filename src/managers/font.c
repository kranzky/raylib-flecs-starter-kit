#include "font.h"

//==============================================================================

static Font _font[MAX_FONTS];

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  for (int i = 0; i < MAX_FONTS; ++i)
  {
    UnloadFont(_font[i]);
  }
}

//------------------------------------------------------------------------------

static void _load(int id, const char *name)
{
  char filename[256];
  sprintf(filename, "./res/fonts/%s.ttf", name);
  _font[id] = LoadFont(filename);
  SetTextureFilter(_font[id].texture, FILTER_POINT);
}

//------------------------------------------------------------------------------

void font_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  _load(FONT_CLOVER, "clover-sans");
}

//------------------------------------------------------------------------------

Font *font_manager_get(FontName id)
{
  return &_font[id];
}
