#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

#include "../defines.h"

typedef enum DataName
{
  DATA_LEVEL_1,
  DATA_LEVEL_2,
  DATA_LEVEL_3,
  DATA_LEVEL_4,
  DATA_LEVEL_5,
  DATA_LEVEL_6,
  DATA_LEVEL_7,
  DATA_LEVEL_8,
  DATA_LEVEL_9,
  MAX_DATA
} DataName;

typedef struct Sheep
{
  Vector2 position;
  Color tint;
  float size;
} Sheep;

typedef struct Data
{
  char *name;
  int num_sheep;
  Sheep sheep[MAX_SHEEP];
} Data;

void data_manager_init(ecs_world_t *world);
Data *data_manager_get(DataName id);

#endif
