#include <raymath.h>
#include <raylib.h>

#include "../components/spatial.h"
#include "../components/tinted.h"

#include "../defines.h"

#include "entity.h"

//==============================================================================

static ecs_world_t *_world = NULL;

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  _world = NULL;
}

//------------------------------------------------------------------------------

void entity_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  ECS_TYPE_DEFINE(world, SceneType, Scene);
  ECS_TYPE_DEFINE(world, LabelType, Label, Spatial, Tinted);
  _world = world;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_scene(SceneName id)
{
  ecs_entity_t e = ecs_new(_world, SceneType);
  ecs_set(_world, e, Scene, {.id = id});
  return e;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_label(FontName id, const char *text, TextAlignment align, TextVerticalAlignment valign, float size, Vector2 position, Color tint)
{
  ecs_entity_t e = ecs_new(_world, LabelType);
  Font *font = font_manager_get(id);
  ecs_set(_world, e, Label, {.font = font, .text = text, .align = align, .valign = valign, .size = size});
  ecs_set(_world, e, Spatial, {.position = position});
  ecs_set(_world, e, Tinted, {.tint = tint});
  return e;
}
