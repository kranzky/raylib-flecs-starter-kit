#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <flecs.h>
#include <raymath.h>

#include "../components/scene.h"
#include "../components/label.h"

#include "../managers/font.h"

ECS_TYPE_DECLARE(SceneType);
ECS_TYPE_DECLARE(LabelType);

void entity_manager_init(ecs_world_t *world);

ecs_entity_t entity_manager_spawn_scene(SceneName id);
ecs_entity_t entity_manager_spawn_label(FontName id, const char *message, TextAlignment align, TextVerticalAlignment valign, float size, Vector2 position, Color tint);

#endif
