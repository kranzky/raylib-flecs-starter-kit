#include <raylib.h>
#include <raymath.h>
#include <flecs.h>

#include "../defines.h"

#include "../components/scene.h"

//==============================================================================

void update_scene(ecs_iter_t *it)
{
  Scene *scene = ecs_column(it, Scene, 1);
  for (int i = 0; i < it->count; ++i)
  {
    TraceLog(LOG_TRACE, "tick");
    switch (scene[i].state)
    {
    case SCENE_STATE_WAITING:
      break;
    case SCENE_STATE_STARTING:
      break;
    case SCENE_STATE_PLAYING:
      break;
    case SCENE_STATE_PAUSED:
      break;
    case SCENE_STATE_STOPPING:
      break;
    case SCENE_STATE_STOPPED:
      break;
    default:
      TraceLog(LOG_WARNING, "bad scene state");
      break;
    }
    scene[i].scene_time += it->delta_time;
    scene[i].state_time += it->delta_time;
    if (scene[i].state == SCENE_STATE_STOPPED)
      ecs_delete(it->world, it->entities[i]);
  }
}
