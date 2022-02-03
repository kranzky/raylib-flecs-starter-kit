#ifndef PHYSICAL_COMPONENT_H
#define PHYSICAL_COMPONENT_H

#include <flecs.h>
#include <chipmunk_structs.h>

typedef enum BodyType
{
  BODY_TYPE_NONE,
  BODY_TYPE_WALL,
  BODY_TYPE_BALL,
  MAX_BODY_TYPES
} BodyType;

typedef struct Joint
{
  cpBody *body;
  cpConstraint *constraint;
} Joint;

typedef struct Physical
{
  BodyType type;
  cpBody *body;
  cpShape *shape;
  Joint joints[9];
  int owner;
  int team;
} Physical;

ECS_COMPONENT_DECLARE(Physical);

#endif
