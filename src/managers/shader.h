#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <raylib.h>
#include <flecs.h>

typedef enum ShaderName
{
  SHADER_DEFAULT,
  MAX_SHADERS
} ShaderName;

void shader_manager_init(ecs_world_t *world);
Shader *shader_manager_get(ShaderName id);

#endif
