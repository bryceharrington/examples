#define EFL_BETA_API_SUPPORT
#include <Eo.h>
#include "eo_multiinherit.h"

typedef struct
{
   int red, green, blue;
} Example_Colored_Data;

EOLIAN static void
_example_colored_color_set(Eo *obj EINA_UNUSED, Example_Colored_Data *pd,
                           int red, int green, int blue)
{
   pd->red = red;
   pd->green = green;
   pd->blue = blue;
}

EOLIAN static void
_example_colored_color_get(Eo *obj EINA_UNUSED, Example_Colored_Data *pd,
                           int *red, int *green, int *blue)
{
   if (red)
     *red = pd->red;
   if (green)
     *green = pd->green;
   if (blue)
     *blue = pd->blue;
}

#include "example_colored.eo.c"
