#ifndef SCENE_COMPONENT_H
#define SCENE_COMPONENT_H

#include <flecs.h>

typedef enum SceneName
{
  SCENE_SPLASH,
  SCENE_TITLE,
  SCENE_LEVEL,
  SCENE_GAME_OVER,
  MAX_SCENES
} SceneName;

typedef enum SceneState
{
  SCENE_STATE_STARTING,
  SCENE_STATE_RUNNING,
  SCENE_STATE_STOPPING,
  MAX_SCENE_STATES
} SceneState;

typedef struct Scene
{
  SceneName id;
  SceneState state;
  float time;
} Scene;

ECS_COMPONENT_DECLARE(Scene);

#endif
