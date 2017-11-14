#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Efl_Core.h>

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
