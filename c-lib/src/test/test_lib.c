#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "suite.h"

START_TEST (lib_test_basic_call)
{
   ck_assert(example_method());
}
END_TEST

void lib_test_basic(TCase *tc)
{
   tcase_add_test(tc, lib_test_basic_call);
}

