#include <raymath.h>
#include <raylib.h>

#include "../components/spatial.h"
#include "../components/tinted.h"
#include "../components/nuklear.h"
#include "../components/renderable.h"
#include "../components/audible.h"
#include "../components/track.h"

#include "entity.h"

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
}

//------------------------------------------------------------------------------

void entity_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);

  ECS_TYPE_DEFINE(world, SceneType, Scene);
  ECS_TYPE_DEFINE(world, LabelType, Label, Spatial, Tinted);
  ECS_TYPE_DEFINE(world, ImageType, Renderable, Spatial, Tinted);

  ECS_TAG_DEFINE(world, DebugTag);
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_scene(ecs_world_t *world, SceneName id, Color color, ShaderName shader_id)
{
  ecs_entity_t entity = ecs_new(world, SceneType);
  Shader *shader = shader_manager_get(shader_id);
  ecs_set(world, entity, Scene, {.id = id, .color = color, .shader = shader});
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_label(ecs_world_t *world, ecs_entity_t parent, FontName id, const char *text, TextAlignment align, TextVerticalAlignment valign, float size, Vector2 position, Color tint)
{
  ecs_entity_t entity = ecs_new(world, LabelType);
  Font *font = font_manager_get(id);
  ecs_set(world, entity, Label, {.font = font, .text = text, .align = align, .valign = valign, .size = size});
  ecs_set(world, entity, Spatial, {.position = position});
  ecs_set(world, entity, Tinted, {.tint = tint});
  ecs_add_pair(world, entity, EcsChildOf, parent);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_debug(ecs_world_t *world, const char *text)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Label, {.text = text});
  ecs_add(world, entity, DebugTag);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_image(ecs_world_t *world, ecs_entity_t parent, TextureName id, float scale, Vector2 position, Color tint)
{
  ecs_entity_t entity = ecs_new(world, ImageType);
  Texture *texture = texture_manager_get(id);
  ecs_set(world, entity, Renderable, {.texture = texture, .scale = scale, .src = (Rectangle){0, 0, texture->width, texture->height}});
  ecs_set(world, entity, Spatial, {.position = position});
  ecs_set(world, entity, Tinted, {.tint = tint});
  ecs_add_pair(world, entity, EcsChildOf, parent);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_sound(ecs_world_t *world, ecs_entity_t parent, SoundName id, float volume)
{
  ecs_entity_t entity = ecs_new(world, 0);
  Sound *sound = sound_manager_get(id);
  ecs_set(world, entity, Audible, {.sound = sound, .volume = volume});
  ecs_add_pair(world, entity, EcsChildOf, parent);
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_music(ecs_world_t *world, MusicName id, float volume)
{
  ecs_entity_t entity = ecs_new(world, 0);
  Music *music = music_manager_get(id);
  ecs_set(world, entity, Track, {.music = music, .volume = volume});
  return entity;
}

//------------------------------------------------------------------------------

ecs_entity_t entity_manager_spawn_transition(ecs_world_t *world, TransitionName id)
{
  ecs_entity_t entity = ecs_new(world, 0);
  ecs_set(world, entity, Transition, {.id = id});
  return entity;
}
