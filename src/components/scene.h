#ifndef SCENE_COMPONENT_H
#define SCENE_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

typedef enum SceneName
{
  SCENE_SPLASH,
  SCENE_TITLE,
  SCENE_LEVEL,
  SCENE_GAME_OVER,
  MAX_SCENES
} SceneName;

typedef struct Scene
{
  SceneName id;
  float time;
  float value;
  Color color;
  Shader *shader;
  Texture *texture;
} Scene;

ECS_COMPONENT_DECLARE(Scene);

#endif
