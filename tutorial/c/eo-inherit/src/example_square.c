#define EFL_BETA_API_SUPPORT
#include <Eo.h>
#include "example_square.eo.h"

#include "eo_inherit.h"

typedef struct
{

} Example_Square_Data;

EOLIAN static void
_example_square_side_set(Eo *obj, Example_Square_Data *pd EINA_UNUSED, int size)
{
   example_rectangle_width_set(obj, size);
   example_rectangle_height_set(obj, size);
}

EOLIAN static int
_example_square_side_get(Eo *obj, Example_Square_Data *pd EINA_UNUSED)
{
   return example_rectangle_width_get(obj);
}

#include "example_square.eo.c"
