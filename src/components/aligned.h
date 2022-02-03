#ifndef ALIGNED_COMPONENT_H
#define ALIGNED_COMPONENT_H

#include <flecs.h>

typedef enum HorizontalAlignment
{
  ALIGN_LEFT,
  ALIGN_CENTRE,
  ALIGN_RIGHT,
  MAX_ALIGN
} HorizontalAlignment;

typedef enum VerticalAlignment
{
  VALIGN_TOP,
  VALIGN_MIDDLE,
  VALIGN_BOTTOM,
  MAX_VALIGN
} VerticalAlignment;

typedef struct Aligned
{
  HorizontalAlignment align;
  VerticalAlignment valign;
} Aligned;

ECS_COMPONENT_DECLARE(Aligned);

#endif
