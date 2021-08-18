#include "shader.h"

//==============================================================================

static Shader _shaders[MAX_SHADERS];

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  for (int i = 0; i < MAX_SHADERS; ++i)
    UnloadShader(_shaders[i]);
}

//------------------------------------------------------------------------------

static void _load(int id, const char *name)
{
  char filename[256];
  sprintf(filename, "./res/shaders/%s.fs", name);
  _shaders[id] = LoadShader(0, TextFormat(filename));
}

//------------------------------------------------------------------------------

void shader_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  _load(SHADER_DEFAULT, "default");
}

//------------------------------------------------------------------------------

Shader *shader_manager_get(ShaderName id)
{
  return &_shaders[id];
}
