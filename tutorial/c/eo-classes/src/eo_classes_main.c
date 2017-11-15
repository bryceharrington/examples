#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>
#include "example_rectangle.eo.h"

Example_Rectangle *
_rect_create()
{
   Example_Rectangle *rectangle;

   rectangle = efl_add(EXAMPLE_RECTANGLE_CLASS, NULL,
                       efl_name_set(efl_added, "Rectangle"),
                       example_rectangle_width_set(efl_added, 5),
                       example_rectangle_height_set(efl_added, 10));

   return rectangle;
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eo *rectangle;

   rectangle = _rect_create();

   printf("Rectangle is %dx%d, area is %d\n",
          example_rectangle_width_get(rectangle),
          example_rectangle_height_get(rectangle),
          example_rectangle_area(rectangle));

   efl_exit(0);
}
EFL_MAIN()

