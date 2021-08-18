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

static inline void _add_parent(ecs_entity_t child, ecs_entity_t parent)
{
  if (parent == 0 || child == 0)
    return;
  ecs_add_entity(_world, child, ECS_CHILDOF | parent);
}

//------------------------------------------------------------------------------

void entity_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);

  ECS_TYPE_DEFINE(world, SceneType, Scene);
  ECS_TYPE_DEFINE(world, LabelType, Label, Spatial, Tinted);

  ECS_TAG_DEFINE(world, DebugTag);

  _world = world;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_scene(SceneName id)
{
  ecs_entity_t entity = ecs_new(_world, SceneType);
  ecs_set(_world, entity, Scene, {.id = id});
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_label(ecs_entity_t parent, FontName id, const char *text, TextAlignment align, TextVerticalAlignment valign, float size, Vector2 position, Color tint)
{
  ecs_entity_t entity = ecs_new(_world, LabelType);
  Font *font = font_manager_get(id);
  ecs_set(_world, entity, Label, {.font = font, .text = text, .align = align, .valign = valign, .size = size});
  ecs_set(_world, entity, Spatial, {.position = position});
  ecs_set(_world, entity, Tinted, {.tint = tint});
  _add_parent(entity, parent);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_debug(const char *text)
{
  ecs_entity_t entity = ecs_new(_world, 0);
  ecs_set(_world, entity, Label, {.text = text});
  ecs_add(_world, entity, DebugTag);
  return entity;
}
