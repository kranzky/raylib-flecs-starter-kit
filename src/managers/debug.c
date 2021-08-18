#include "../components/debug.h"

#include "debug.h"

//==============================================================================

static char *_version = "DEBUG BUILD";

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
#ifdef RELEASE
  UnloadFileText((unsigned char *)_version);
#endif
}

//------------------------------------------------------------------------------

void debug_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);

  bool enabled = false;

#ifdef RELEASE
  _version = LoadFileText("./res/VERSION");
#endif
#ifdef DEBUG
  enabled = true;
#endif

  ecs_singleton_set(world, Debug, {.version = _version, .enabled = enabled});
}
