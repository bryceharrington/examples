#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

static void
_value_int()
{
   Eina_Value int_val;
   char *str;
   int i;

   eina_value_setup(&int_val, EINA_VALUE_TYPE_INT);
   eina_value_set(&int_val, 123);
   eina_value_get(&int_val, &i);
   printf("int_val value is %d\n", i);

   str = eina_value_to_string(&int_val);
   printf("int_val to string is \"%s\"\n", str);
   free(str); // it was allocated by eina_value_to_string()
   eina_value_flush(&int_val);
}

static void
_value_string()
{
   Eina_Value str_val;
   const char *str;
   char *newstr;

   eina_value_setup(&str_val, EINA_VALUE_TYPE_STRING);
   eina_value_set(&str_val, "My string");
   eina_value_get(&str_val, &str);
   printf("str_val value is \"%s\" (pointer: %p)\n", str, str);

   newstr = eina_value_to_string(&str_val);
   printf("str_val to string is \"%s\" (pointer: %p)\n", newstr, newstr);
   free(newstr); // it was allocated by eina_value_to_string()
   eina_value_flush(&str_val);
}

static void
_value_convert()
{
   Eina_Value str_val, int_val;
   int i;
   char *str;

   eina_value_setup(&str_val, EINA_VALUE_TYPE_STRING);
   eina_value_setup(&int_val, EINA_VALUE_TYPE_INT);

   // convert from int to string:
   eina_value_set(&int_val, 123);
   eina_value_get(&int_val, &i);
   eina_value_convert(&int_val, &str_val);
   eina_value_get(&str_val, &str);
   printf("int_val was %d, converted to string is \"%s\"\n", i, str);

   // and the other way around!
   eina_value_set(&str_val, "33");
   eina_value_get(&str_val, &str);
   eina_value_convert(&str_val, &int_val);
   eina_value_get(&int_val, &i);
   printf("str_val was \"%s\", converted to int is %d\n", str, i);

   eina_value_flush(&str_val);
   eina_value_flush(&int_val);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   _value_int();
   printf("\n");

   _value_string();
   printf("\n");

   _value_convert();

   efl_exit(0);
}
EFL_MAIN()

