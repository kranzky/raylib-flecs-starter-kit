#ifndef WIDGET_COMPONENT_H
#define WIDGET_COMPONENT_H

#include <flecs.h>

typedef enum WidgetType
{
  WIDGET_SEPARATOR,
  WIDGET_LABEL,
  WIDGET_BUTTON,
  WIDGET_SLIDER,
  MAX_WIDGETS
} WidgetType;

typedef struct Widget Widget;
typedef void (*widgetCallback)(ecs_world_t *world, Widget *widget);

typedef struct Widget
{
  WidgetType type;
  const char *name;
  float value;
  widgetCallback callback;
} Widget;

ECS_COMPONENT_DECLARE(Widget);

#endif
