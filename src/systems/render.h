#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <flecs.h>

void refresh_display(ecs_iter_t *it);
void render_scene(ecs_iter_t *it);
void render_labels(ecs_iter_t *it);
void render_images(ecs_iter_t *it);
void composite_display(ecs_iter_t *it);

#endif
