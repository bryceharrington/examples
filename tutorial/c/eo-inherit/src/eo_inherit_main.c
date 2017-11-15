#include "eo_inherit.h"

Example_Shape *
_rectangle_create()
{
   Example_Rectangle *rectangle;

   rectangle = efl_add(EXAMPLE_RECTANGLE_CLASS, NULL,
                       efl_name_set(efl_added, "Rectangle"),
                       example_rectangle_width_set(efl_added, 5),
                       example_rectangle_height_set(efl_added, 10));

   return rectangle;
}

Example_Shape *
_square_create()
{
   Example_Square *square;

   square = efl_add(EXAMPLE_SQUARE_CLASS, NULL,
                    efl_name_set(efl_added, "Square"),
                    example_square_side_set(efl_added, 7));

   return square;
}

Example_Shape *
_circle_create()
{
   Example_Circle *circle;

   circle = efl_add(EXAMPLE_CIRCLE_CLASS, NULL,
                    efl_name_set(efl_added, "Circle"),
                    example_circle_radius_set(efl_added, 5));

   return circle;
}

void
_shape_print(Example_Shape *shape)
{
   printf("Shape named %s has area %d\n", efl_name_get(shape), example_shape_area(shape));
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eo *shape;

   shape = _rectangle_create();
   _shape_print(shape);

   shape = _square_create();
   _shape_print(shape);

   shape = _circle_create();
   _shape_print(shape);

   efl_exit(0);
}
EFL_MAIN()

