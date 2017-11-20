#define EFL_BETA_API_SUPPORT
#include <Eo.h>
#include "example_square.eo.h"
#include "example_rectangle.eo.h"

typedef struct
{

} Example_Square_Data;

EOLIAN static void
_example_square_example_rectangle_width_set(Eo *obj, Example_Square_Data *pd EINA_UNUSED, int width)
{
   example_rectangle_width_set(efl_super(obj, EXAMPLE_SQUARE_CLASS), width);
   example_rectangle_height_set(efl_super(obj, EXAMPLE_SQUARE_CLASS), width);
}

EOLIAN static void
_example_square_example_rectangle_height_set(Eo *obj, Example_Square_Data *pd EINA_UNUSED, int height)
{
   example_rectangle_width_set(efl_super(obj, EXAMPLE_SQUARE_CLASS), height);
   example_rectangle_height_set(efl_super(obj, EXAMPLE_SQUARE_CLASS), height);
}

#include "example_square.eo.c"
