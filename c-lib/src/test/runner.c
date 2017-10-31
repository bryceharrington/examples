#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <check.h>
#include <Elementary.h>

#include <Example.h>
#include "suite.h"

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev)
{
   Efl_Loop_Arguments *args = ev->info;
   Suite *s;
   SRunner *sr;
   TCase *tc = NULL;
   char *test = NULL;
   unsigned int i;
   int failed_count = -1;

   if (eina_array_count(args->argv) > 0)
     {
        test = eina_array_data_get(args->argv, 0);

        if (!strcmp(test, "--list"))
          {
             fprintf(stdout, "Available tests :\n");
             for (i = 0; i < sizeof (tests) / sizeof (tests[0]); i++)
               fprintf(stdout, "\t%s\n", tests[i].name);

             efl_exit(0);
             return;
          }
     }

   s = suite_create("Example");
   for (i = 0; i < sizeof (tests) / sizeof (tests[0]); i++)
     {
        if (test && strcmp(tests[i].name, test))
          continue ;

        tc =  tcase_create(tests[i].name);
        tcase_set_timeout(tc, 0);

        tests[i].build(tc);
        suite_add_tcase(s, tc);
     }

   sr = srunner_create(s);
   srunner_set_xml(sr, PACKAGE_BUILD_DIR "/check-results.xml");

   srunner_run_all(sr, CK_ENV);
   failed_count = srunner_ntests_failed(sr);
   srunner_free(sr);

   efl_exit((failed_count == 0) ? 0 : 255);
}
EFL_MAIN();
