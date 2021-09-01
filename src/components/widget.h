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

typedef struct Widget
{
  WidgetType type;
  const char *name;
  void (*callback)(struct Widget *widget);
} Widget;

ECS_COMPONENT_DECLARE(Widget);

#endif
