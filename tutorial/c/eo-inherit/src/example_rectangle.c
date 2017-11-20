#define EFL_BETA_API_SUPPORT
#include <Eo.h>
#include "example_rectangle.eo.h"

typedef struct
{
   int width, height;
} Example_Rectangle_Data;

EOLIAN static void
_example_rectangle_width_set(Eo *obj EINA_UNUSED, Example_Rectangle_Data *pd, int width)
{
   pd->width = width;
}

EOLIAN static int
_example_rectangle_width_get(Eo *obj EINA_UNUSED, Example_Rectangle_Data *pd)
{
   return pd->width;
}

EOLIAN static void
_example_rectangle_height_set(Eo *obj EINA_UNUSED, Example_Rectangle_Data *pd, int height)
{
   pd->height = height;
}

EOLIAN static int
_example_rectangle_height_get(Eo *obj EINA_UNUSED, Example_Rectangle_Data *pd)
{
   return pd->height;
}

EOLIAN static int
_example_rectangle_area(Eo *obj EINA_UNUSED, Example_Rectangle_Data *pd)
{
   return pd->width * pd->height;
}

#include "example_rectangle.eo.c"
