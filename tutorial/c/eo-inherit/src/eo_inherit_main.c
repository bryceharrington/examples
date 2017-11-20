#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Eina.h>
#include <Efl_Core.h>
#include "example_rectangle.eo.h"
#include "example_square.eo.h"

Example_Rectangle *
_rectangle_create()
{
   Example_Rectangle *rectangle;

   rectangle = efl_add(EXAMPLE_RECTANGLE_CLASS, NULL,
                       efl_name_set(efl_added, "Rectangle"),
                       example_rectangle_width_set(efl_added, 5),
                       example_rectangle_height_set(efl_added, 10));

   return rectangle;
}

Example_Square *
_square_create()
{
   Example_Square *square;

   square = efl_add(EXAMPLE_SQUARE_CLASS, NULL,
                    efl_name_set(efl_added, "Square"),
                    example_rectangle_width_set(efl_added, 7));

   return square;
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eo *rectangle, *square;

   rectangle = _rectangle_create();

   printf("Rectangle is %dx%d, area is %d\n",
          example_rectangle_width_get(rectangle),
          example_rectangle_height_get(rectangle),
          example_rectangle_area(rectangle));

   efl_unref(rectangle);

   square = _square_create();

   printf("Square is %dx%d, area is %d\n",
          example_rectangle_width_get(square),
          example_rectangle_height_get(square),
          example_rectangle_area(square));

   efl_unref(square);

   efl_exit(0);
}
EFL_MAIN()

