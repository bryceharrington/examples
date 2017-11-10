#define EFL_BETA_API_SUPPORT
#include <Eo.h>
#include "example_square.eo.h"

#include "eo_classes.h"

typedef struct
{
   int size;
} Example_Square_Data;

EOLIAN static void
_example_square_side_set(Eo *obj EINA_UNUSED, Example_Square_Data *pd, int size)
{
   pd->size = size;
}

EOLIAN static int
_example_square_side_get(Eo *obj EINA_UNUSED, Example_Square_Data *pd)
{
   return pd->size;
}

EOLIAN static void
_example_square_example_rectangle_width_set(Eo *obj EINA_UNUSED, Example_Square_Data *pd, int width)
{
   pd->size = width;
}

EOLIAN static int
_example_square_example_rectangle_width_get(Eo *obj EINA_UNUSED, Example_Square_Data *pd)
{
   return pd->size;
}

EOLIAN static void
_example_square_example_rectangle_height_set(Eo *obj EINA_UNUSED, Example_Square_Data *pd, int height)
{
   pd->size = height;
}

EOLIAN static int
_example_square_example_rectangle_height_get(Eo *obj EINA_UNUSED, Example_Square_Data *pd)
{
   return pd->size;
}

EOLIAN static int
_example_square_example_shape_area(Eo *obj EINA_UNUSED, Example_Square_Data *pd)
{
   return pd->size * pd->size;
}

EOLIAN static void
_example_square_class_constructor(Efl_Class *klass EINA_UNUSED)
{
}

EOLIAN static void
_example_square_class_destructor(Efl_Class *klass EINA_UNUSED)
{
}

#include "example_square.eo.c"
