#ifndef TIME_SYSTEM_H
#define TIME_SYSTEM_H

#include <flecs.h>

void progress_time(ecs_iter_t *it);
void transition(ecs_iter_t *it);
void state_machine(ecs_iter_t *it);

#endif
