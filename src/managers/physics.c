#include <flecs.h>
#include <raylib.h>

#include "../helpers.h"
#include "../defines.h"

#include "../components/physical.h"
#include "../components/renderable.h"
#include "../components/tinted.h"
#include "../components/collision.h"
#include "../components/spatial.h"
#include "../components/animated.h"
#include "../components/stateful.h"
#include "../components/aligned.h"

#include "../managers/texture.h"
#include "../managers/entity.h"
#include "../managers/system.h"

#include "physics.h"

//==============================================================================

static void _fini(ecs_world_t *world, void *context)
{
  Physics *physics = ecs_singleton_get_mut(world, Physics);
  cpSpaceFree(physics->space);
  physics->space = NULL;
  ecs_singleton_modified(world, Physics);
}

//------------------------------------------------------------------------------

static inline void *_store_entity(ecs_entity_t value)
{
  ecs_entity_t *pointer = RL_MALLOC(sizeof(ecs_entity_t));
  assert(pointer != NULL);
  *pointer = value;
  return (void *)pointer;
}

//------------------------------------------------------------------------------

static inline ecs_entity_t _get_entity(void *pointer)
{
  return *(ecs_entity_t *)pointer;
}

//------------------------------------------------------------------------------

static inline void _delete_entity(void *pointer)
{
  assert(pointer != NULL);
  RL_FREE((ecs_entity_t *)pointer);
}

//------------------------------------------------------------------------------

void _destroy_physical(ecs_iter_t *it)
{
  Physical *physical = ecs_term(it, Physical, 1);
  const Physics *physics = ecs_singleton_get(it->world, Physics);
  for (int i = 0; i < it->count; ++i)
  {
    for (int j = 8; j >= 0; --j)
    {
      if (physical[i].joints[j].constraint != NULL)
      {
        if (physical[i].joints[j].constraint->space != NULL)
          cpSpaceRemoveConstraint(physics->space, physical[i].joints[j].constraint);
        cpConstraintFree(physical[i].joints[j].constraint);
        physical[i].joints[j].constraint = NULL;
      }
      if (physical[i].joints[j].body != NULL)
      {
        if (physical[i].joints[j].body->space != NULL)
          cpSpaceRemoveBody(physics->space, physical[i].joints[j].body);
        cpBodyFree(physical[i].joints[j].body);
        physical[i].joints[j].body = NULL;
      }
    }
    if (physical[i].shape != NULL)
    {
      if (physical[i].shape->space != NULL)
        cpSpaceRemoveShape(physics->space, physical[i].shape);
      if (physical[i].shape->userData != NULL)
        _delete_entity(physical[i].shape->userData);
      cpShapeFree(physical[i].shape);
      physical[i].shape = NULL;
    }
    if (physical[i].body != NULL)
    {
      if (physical[i].body->space != NULL)
        cpSpaceRemoveBody(physics->space, physical[i].body);
      cpBodyFree(physical[i].body);
      physical[i].body = NULL;
    }
  }
}

//------------------------------------------------------------------------------

static void _handle_collision(cpArbiter *arb, cpSpace *space, void *data)
{
  if (!cpArbiterIsFirstContact(arb))
    return;
  cpShape *shape[2];
  cpArbiterGetShapes(arb, &shape[0], &shape[1]);
  ecs_world_t *world = space->userData;
  ecs_entity_t entity = ecs_new(world, 0);
  float energy = cpArbiterTotalKE(arb);
  ecs_set(world, entity, Collision, {.energy = energy, .entities = {_get_entity(shape[0]->userData), _get_entity(shape[1]->userData)}, .normal = _to_vector(cpArbiterGetNormal(arb)), .contacts = {_to_vector(cpArbiterGetPointA(arb, 0)), _to_vector(cpArbiterGetPointB(arb, 0))}});
}

//------------------------------------------------------------------------------

void physics_manager_init(ecs_world_t *world)
{
  ecs_atfini(world, _fini, NULL);

  cpSpace *space = cpSpaceNew();
  ecs_singleton_set(world, Physics, {.space = space});

  cpSpaceSetCollisionSlop(space, 0.02);

  ECS_TRIGGER(world, _destroy_physical, EcsOnRemove, Physical);

  cpCollisionHandler *handler = NULL;
  handler = cpSpaceAddCollisionHandler(space, BODY_TYPE_BALL, BODY_TYPE_WALL);
  handler->postSolveFunc = _handle_collision;
}

//------------------------------------------------------------------------------

void physics_manager_set_properties(ecs_world_t *world, float gravity, float damping)
{
  const Physics *physics = ecs_singleton_get(world, Physics);
  cpSpaceSetGravity(physics->space, _from_vector((Vector2){0, gravity}));
  cpSpaceSetDamping(physics->space, damping);
}

//------------------------------------------------------------------------------

void physics_ball(ecs_world_t *world, ecs_entity_t parent, float mass, float radius, Vector2 position)
{
  float scale = 0.25;
  position = Vector2Scale(position, scale);
  const Physics *physics = ecs_singleton_get(world, Physics);
  ecs_entity_t entity = ecs_new(world, 0);
  cpBody *body = cpBodyNew(mass, cpMomentForCircle(mass, 0, radius * 0.9, cpvzero));
  cpBodySetPosition(body, _from_vector(position));
  cpSpaceAddBody(physics->space, body);
  cpShape *shape = cpCircleShapeNew(body, radius * 0.9, cpvzero);
  cpShapeSetUserData(shape, _store_entity(entity));
  cpShapeSetFriction(shape, 0.3);
  cpShapeSetElasticity(shape, 0.8);
  cpShapeSetCollisionType(shape, BODY_TYPE_BALL);
  cpSpaceAddShape(physics->space, shape);
  Texture *texture = texture_manager_get(TEXTURE_SHEEP);
  ecs_set(world, entity, Physical, {.body = body, .shape = shape, .type = BODY_TYPE_BALL});
  ecs_set(world, entity, Renderable, {.texture = texture, .scale = scale * radius * 0.014, .src = (Rectangle){0, 0, texture->width, texture->height}});
  ecs_set(world, entity, Tinted, {.tint = WHITE});
  ecs_add_pair(world, entity, EcsChildOf, parent);
}

//------------------------------------------------------------------------------

void physics_line(ecs_world_t *world, ecs_entity_t parent, Vector2 from, Vector2 to, float radius)
{
  const Physics *physics = ecs_singleton_get(world, Physics);
  ecs_entity_t entity = ecs_new(world, 0);
  cpBody *body = cpSpaceGetStaticBody(physics->space);
  cpShape *shape = cpSegmentShapeNew(body, _from_vector(from), _from_vector(to), radius);
  cpShapeSetUserData(shape, _store_entity(entity));
  cpShapeSetFriction(shape, 0.7);
  cpShapeSetElasticity(shape, 0.8);
  cpShapeSetCollisionType(shape, BODY_TYPE_WALL);
  cpSpaceAddShape(physics->space, shape);
  ecs_set(world, entity, Physical, {.body = NULL, .shape = shape, .type = BODY_TYPE_WALL});
  ecs_add_pair(world, entity, EcsChildOf, parent);
}

//------------------------------------------------------------------------------

void physics_box(ecs_world_t *world, ecs_entity_t parent, Vector2 position)
{
  const Physics *physics = ecs_singleton_get(world, Physics);
  ecs_entity_t entity = ecs_new(world, 0);
  cpBody *body = cpSpaceGetStaticBody(physics->space);
  float size = 2.0;
  cpVect vertices[] = {{position.x - size, position.y - size}, {position.x + size, position.y - size}, {position.x + size, position.y + size}, {position.x - size, position.y + size}};
  cpShape *shape = cpPolyShapeNew(body, 4, vertices, cpTransformIdentity, 0.0);
  cpShapeSetUserData(shape, _store_entity(entity));
  cpShapeSetFriction(shape, 0.7);
  cpShapeSetElasticity(shape, 0.8);
  cpShapeSetCollisionType(shape, BODY_TYPE_WALL);
  cpSpaceAddShape(physics->space, shape);
  ecs_set(world, entity, Physical, {.body = NULL, .shape = shape, .type = BODY_TYPE_WALL});
  ecs_add_pair(world, entity, EcsChildOf, parent);
}

//------------------------------------------------------------------------------

void physics_wedge(ecs_world_t *world, ecs_entity_t parent, Vector2 position, int corner)
{
  const Physics *physics = ecs_singleton_get(world, Physics);
  ecs_entity_t entity = ecs_new(world, 0);
  cpBody *body = cpSpaceGetStaticBody(physics->space);
  float size = 2.0;
  cpVect vertices[] = {{position.x - size, position.y - size}, {position.x + size, position.y - size}, {position.x + size, position.y + size}, {position.x - size, position.y + size}};
  cpVect wedge[3] = {0};
  int j = 0;
  for (int i = 0; i < 4; ++i)
  {
    if (i == corner)
      continue;
    wedge[j] = vertices[i];
    ++j;
  }
  cpShape *shape = cpPolyShapeNew(body, 3, wedge, cpTransformIdentity, 0.0);
  cpShapeSetUserData(shape, _store_entity(entity));
  cpShapeSetFriction(shape, 0.7);
  cpShapeSetElasticity(shape, 0.8);
  cpShapeSetCollisionType(shape, BODY_TYPE_WALL);
  cpSpaceAddShape(physics->space, shape);
  ecs_set(world, entity, Physical, {.body = NULL, .shape = shape, .type = BODY_TYPE_WALL});
  ecs_add_pair(world, entity, EcsChildOf, parent);
}

//==============================================================================

#ifdef DEBUG

static void _draw_dot(cpFloat size, cpVect position, cpSpaceDebugColor color, cpDataPointer data)
{
  DrawCircleV(_to_vector(position), size * 0.5, _to_color(color));
}

//------------------------------------------------------------------------------

static void _draw_circle(cpVect position, cpFloat angle, cpFloat radius, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
  Vector2 pos = _to_vector(position);
  Vector2 offset = Vector2Rotate((Vector2){0, 1}, RAD2DEG * angle);
  DrawCircleV(pos, radius + 0.1, _to_color(outline));
  DrawCircleV(pos, radius - 0.1, _to_color(fill));
  DrawLineV(pos, Vector2Add(pos, Vector2Scale(offset, radius * 0.75)), _to_color(outline));
}

//------------------------------------------------------------------------------

static void _draw_segment(cpVect from, cpVect to, cpSpaceDebugColor color, cpDataPointer data)
{
  DrawLineV(_to_vector(from), _to_vector(to), _to_color(color));
}

//------------------------------------------------------------------------------

static void _draw_fat_segment(cpVect from, cpVect to, cpFloat radius, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
  DrawLineEx(_to_vector(from), _to_vector(to), 2 * radius + 0.1, _to_color(outline));
  DrawLineEx(_to_vector(from), _to_vector(to), 2 * radius - 0.1, _to_color(fill));
}

//------------------------------------------------------------------------------

static void _draw_polygon(int length, const cpVect *points, cpFloat radius, cpSpaceDebugColor outline, cpSpaceDebugColor fill, cpDataPointer data)
{
  Vector2 vertices[length + 2];
  vertices[0] = _to_vector(cpCentroidForPoly(length, points));
  for (int i = 0; i < length; ++i)
    vertices[i + 1] = _to_vector(points[length - i - 1]);
  vertices[length + 1] = vertices[1];
  DrawTriangleFan(vertices, length + 2, _to_color(fill));
  for (int i = 1; i < length + 1; ++i)
    DrawLineEx(vertices[i], vertices[i + 1], 2 * radius + 0.2, _to_color(outline));
}

//------------------------------------------------------------------------------

static inline cpSpaceDebugColor RGBAColor(float r, float g, float b, float a)
{
  cpSpaceDebugColor color = {r, g, b, a};
  return color;
}

//------------------------------------------------------------------------------

static inline cpSpaceDebugColor LAColor(float l, float a)
{
  cpSpaceDebugColor color = {l, l, l, a};
  return color;
}

//------------------------------------------------------------------------------

static cpSpaceDebugColor Colors[] = {
    {0xb5 / 255.0f, 0x89 / 255.0f, 0x00 / 255.0f, 1.0f},
    {0xcb / 255.0f, 0x4b / 255.0f, 0x16 / 255.0f, 1.0f},
    {0xdc / 255.0f, 0x32 / 255.0f, 0x2f / 255.0f, 1.0f},
    {0xd3 / 255.0f, 0x36 / 255.0f, 0x82 / 255.0f, 1.0f},
    {0x6c / 255.0f, 0x71 / 255.0f, 0xc4 / 255.0f, 1.0f},
    {0x26 / 255.0f, 0x8b / 255.0f, 0xd2 / 255.0f, 1.0f},
    {0x2a / 255.0f, 0xa1 / 255.0f, 0x98 / 255.0f, 1.0f},
    {0x85 / 255.0f, 0x99 / 255.0f, 0x00 / 255.0f, 1.0f},
};

static cpSpaceDebugColor _shape_colour(cpShape *shape, cpDataPointer data)
{
  if (cpShapeGetSensor(shape))
  {
    return LAColor(1.0f, 0.1f);
  }
  else
  {
    cpBody *body = cpShapeGetBody(shape);

    if (cpBodyIsSleeping(body))
    {
      return RGBAColor(0x58 / 255.0f, 0x6e / 255.0f, 0x75 / 255.0f, 1.0f);
    }
    else if (body->sleeping.idleTime > shape->space->sleepTimeThreshold)
    {
      return RGBAColor(0x93 / 255.0f, 0xa1 / 255.0f, 0xa1 / 255.0f, 1.0f);
    }
    else
    {
      uint32_t val = (uint32_t)shape->hashid;

      // scramble the bits up using Robert Jenkins' 32 bit integer hash function
      val = (val + 0x7ed55d16) + (val << 12);
      val = (val ^ 0xc761c23c) ^ (val >> 19);
      val = (val + 0x165667b1) + (val << 5);
      val = (val + 0xd3a2646c) ^ (val << 9);
      val = (val + 0xfd7046c5) + (val << 3);
      val = (val ^ 0xb55a4f09) ^ (val >> 16);
      return Colors[val & 0x7];
    }
  }
}

//------------------------------------------------------------------------------

static cpSpaceDebugDrawOptions _options = {
    _draw_circle,
    _draw_segment,
    _draw_fat_segment,
    _draw_polygon,
    _draw_dot,
    CP_SPACE_DEBUG_DRAW_SHAPES | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS,
    {0xEE / 255.0f, 0xE8 / 255.0f, 0xD5 / 255.0f, 1.0f},
    _shape_colour,
    {0.0f, 0.75f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f, 1.0f},
    NULL};

cpSpaceDebugDrawOptions *physics_debug_options(void)
{
  return &_options;
}

#endif
