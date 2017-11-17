#define EFL_BETA_API_SUPPORT
#include <Eo.h>
#include "example_circle.eo.h"

#include "eo_multiinherit.h"

typedef struct
{
   int radius;
} Example_Circle_Data;

EOLIAN static void
_example_circle_radius_set(Eo *obj EINA_UNUSED, Example_Circle_Data *pd, int radius)
{
   pd->radius = radius;
}

EOLIAN static int
_example_circle_radius_get(Eo *obj EINA_UNUSED , Example_Circle_Data *pd)
{
   return pd->radius;
}

EOLIAN static int
_example_circle_example_shape_area(Eo *obj EINA_UNUSED, Example_Circle_Data *pd)
{
   return (int)(pd->radius * pd->radius * 3.14159f);
}

#include "example_circle.eo.c"
