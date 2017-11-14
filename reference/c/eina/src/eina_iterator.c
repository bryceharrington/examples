#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Efl_Core.h>

static Eina_Bool
_print_item(const void *container EINA_UNUSED, void *data, void *fdata EINA_UNUSED)
{
   printf("%s\n", (char*)data);
   return EINA_TRUE;
}

static void
_print_iterator(Eina_Iterator *it)
{
   printf("--iterator start--\n");
   eina_iterator_foreach(it, _print_item, NULL);
   printf("-- iterator end --\n");
}

static Eina_Array *
_create_array()
{
   const char *strings[] =
   {
      "name strings",
      "husker",
      "starbuck",
      "boomer"
   };

   Eina_Array *array;
   int i;

   array = eina_array_new(4);
   for (i = 0; i < 4; i++)
     eina_array_push(array, strings[i]);

   return array;
}

static Eina_List *
_create_list()
{
   const char *more_strings[] = {
      "sentence strings",
      "what do your hear?",
      "nothing but the rain",
      "then grab your gun and bring the cat in"
   };

   Eina_List *list = NULL;
   int i;

   for (i = 0; i < 4; i++)
     list = eina_list_append(list, more_strings[i]);

   return list;
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eina_Array *array;
   Eina_List *list;
   Eina_Iterator *it;
   char *s;

   // create an eina_array and iterate through it's contents
   array = _create_array();
   it = eina_array_iterator_new(array);
   _print_iterator(it);
   eina_iterator_free(it);
   eina_array_free(array);

   // perform the same iteration with an eina_list but without
   // using a callback.
   list = _create_list();
   it = eina_list_iterator_new(list);
   EINA_ITERATOR_FOREACH(it, s)
     _print_item(eina_iterator_container_get(it), s, NULL);
   eina_iterator_free(it);
   eina_list_free(list);

   efl_exit(0);
}
EFL_MAIN()
