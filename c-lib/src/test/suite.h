#ifndef _SUITE_H
#define _SUITE_H

#include <check.h>

#include <Example.h>

void lib_test_basic(TCase *tc);

static const struct {
   const char *name;
   void (*build)(TCase *tc);
} tests[] = {
  { "basic", lib_test_basic },
};

#endif /* _SUITE_H */
