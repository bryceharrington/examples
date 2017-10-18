#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev)
{
   Efl_Loop_Arguments *args = ev->info;

   if (eina_array_count(args->argv) == 0)
     {
        printf("Hello World!\n");
     }
   else
     {
        printf("Hello %s!\n", (char *) eina_array_data_get(args->argv, 0));
     }

   efl_exit(0);
}
EFL_MAIN()

