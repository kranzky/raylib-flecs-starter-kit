#include <raymath.h>
#include <chipmunk.h>

//==============================================================================

static inline cpVect _from_vector(Vector2 position)
{
  return (cpVect){position.x, position.y};
}

//------------------------------------------------------------------------------

static inline Vector2 _to_vector(cpVect position)
{
  return (Vector2){position.x, position.y};
}

//------------------------------------------------------------------------------

static inline Color _to_color(cpSpaceDebugColor color)
{
  return ColorFromNormalized((Vector4){color.r, color.g, color.b, color.a});
}
