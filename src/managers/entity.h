#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <flecs.h>
#include <raymath.h>

#include "../components/scene.h"
#include "../components/label.h"
#include "../components/aligned.h"

#include "../managers/font.h"
#include "../managers/texture.h"
#include "../managers/sound.h"
#include "../managers/music.h"
#include "../managers/shader.h"

ECS_TYPE_DECLARE(SceneType);
ECS_TYPE_DECLARE(LabelType);
ECS_TYPE_DECLARE(ImageType);

ECS_TAG_DECLARE(DebugTag);

void entity_manager_init(ecs_world_t *world);

ecs_entity_t entity_manager_spawn_scene(ecs_world_t *world, SceneName id, Color color, ShaderName shader_id, TextureName texture_id);
ecs_entity_t entity_manager_spawn_label(ecs_world_t *world, ecs_entity_t parent, FontName id, const char *message, HorizontalAlignment align, VerticalAlignment valign, float size, Vector2 position, Color tint);
ecs_entity_t entity_manager_spawn_debug(ecs_world_t *world, const char *message);
ecs_entity_t entity_manager_spawn_image(ecs_world_t *world, ecs_entity_t parent, TextureName id, float scale, Vector2 position, Color tint);
ecs_entity_t entity_manager_spawn_sound(ecs_world_t *world, ecs_entity_t parent, SoundName id, float volume);
ecs_entity_t entity_manager_spawn_music(ecs_world_t *world, MusicName id, float volume);
ecs_entity_t entity_manager_spawn_viewport(ecs_world_t *world, ecs_entity_t parent, Vector2 size, Rectangle dst, Color background);

#endif
