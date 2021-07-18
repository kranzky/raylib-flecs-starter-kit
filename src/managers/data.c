#include <cJSON.h>

#include "../defines.h"

#include "data.h"

//==============================================================================

static Data _data[MAX_DATA] = {0};

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
}

//------------------------------------------------------------------------------

void _load_data(DataName id, const char *filename)
{
  char *blob = LoadFileText(filename);
  if (blob == NULL)
  {
    TraceLog(LOG_ERROR, "Cannot find %s", filename);
    return;
  }
  cJSON *data = cJSON_Parse((const char *)blob);
  if (data == NULL)
  {
    const char *error = cJSON_GetErrorPtr();
    if (error != NULL)
      TraceLog(LOG_ERROR, error);
    UnloadFileText((unsigned char *)blob);
    return;
  }
  float vector[3];
  int i;
  const cJSON *node = NULL;
  const cJSON *item = NULL;
  node = cJSON_GetObjectItemCaseSensitive(data, "sheep");
  int b = 0;
  cJSON_ArrayForEach(item, node)
  {
    cJSON *array = NULL;
    cJSON *value = NULL;
    array = cJSON_GetObjectItemCaseSensitive(item, "position");
    i = 0;
    cJSON_ArrayForEach(value, array)
    {
      vector[i] = value->valuedouble;
      ++i;
    }
    Vector2 position = (Vector2){vector[0], vector[1]};
    value = cJSON_GetObjectItemCaseSensitive(item, "tint");
    Color tint = GetColor((unsigned int)value->valuedouble);
    value = cJSON_GetObjectItemCaseSensitive(item, "size");
    float size = value->valuedouble;
    if (b < MAX_SHEEP)
    {
      _data[id].sheep[b] = (Sheep){.position = position, .tint = tint, .size = size};
      ++b;
    }
  }
  UnloadFileText((unsigned char *)blob);
  cJSON_Delete(data);
}

//------------------------------------------------------------------------------

static void _load(DataName id, const char *name)
{
  char filename[256];
  sprintf(filename, "./res/data/%s.json", name);
  _load_data(id, filename);
}

//------------------------------------------------------------------------------

void data_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);
  _load(DATA_LEVEL_1, "level1");
  _load(DATA_LEVEL_2, "level2");
  _load(DATA_LEVEL_3, "level3");
  _load(DATA_LEVEL_4, "level4");
  _load(DATA_LEVEL_5, "level5");
  _load(DATA_LEVEL_6, "level6");
  _load(DATA_LEVEL_7, "level7");
  _load(DATA_LEVEL_8, "level8");
  _load(DATA_LEVEL_9, "level9");
}

//------------------------------------------------------------------------------

Data *data_manager_get(DataName id)
{
  return &_data[id];
}
