#ifndef AUDIBLE_COMPONENT_H
#define AUDIBLE_COMPONENT_H

#include <flecs.h>

#include "../managers/sound.h"

typedef struct Audible
{
  Sound *sound;
  float volume;
} Audible;

ECS_COMPONENT_DECLARE(Audible);

#endif
