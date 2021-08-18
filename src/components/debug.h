#ifndef DEBUG_COMPONENT_H
#define DEBUG_COMPONENT_H

#include <flecs.h>

typedef struct Debug
{
  bool enabled;
  const char *version;
} Debug;

ECS_COMPONENT_DECLARE(Debug);

#endif
