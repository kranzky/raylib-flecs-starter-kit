#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <flecs.h>
#include <raymath.h>

#include "../components/scene.h"
#include "../components/label.h"

#include "../managers/font.h"

ECS_TYPE_DECLARE(SceneType);
ECS_TYPE_DECLARE(LabelType);

ECS_TAG_DECLARE(DebugTag);

void entity_manager_init(ecs_world_t *world);

ecs_entity_t entity_manager_spawn_scene(ecs_world_t *world, SceneName id);
ecs_entity_t entity_manager_spawn_label(ecs_world_t *world, ecs_entity_t parent, FontName id, const char *message, TextAlignment align, TextVerticalAlignment valign, float size, Vector2 position, Color tint);
ecs_entity_t entity_manager_spawn_window(ecs_world_t *world, ecs_entity_t parent, const char *name, int x, int y, int width, int height);
ecs_entity_t entity_manager_spawn_debug(ecs_world_t *world, const char *message);

#endif
