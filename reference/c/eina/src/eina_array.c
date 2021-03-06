#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

/*
 * Eina Array examples.
 *
 * These examples demonstrate how to work with eina_array data and methods.
 * We use a simple array of strings to initialise our eina_array before
 * performing various mutations and printing the results.
 */

static Eina_Array *
_array_create()
{
   // some content to populate our array
   const char *names[] =
   {
      "helo", "hera", "starbuck", "kat", "boomer",
      "hotdog", "longshot", "jammer", "crashdown", "hardball",
      "duck", "racetrack", "apolo", "husker", "freaker",
      "skulls", "bulldog", "flat top", "hammerhead", "gonzo"
   };

   Eina_Array *array;
   unsigned int i;

   // set up an array with a growth step to give a little headroom
   array = eina_array_new(25);
   for (i = 0; i < 20; i++)
     eina_array_push(array, strdup(names[i]));

   return array;
}

static void _array_free(Eina_Array *array)
{
   Eina_Array_Iterator iterator;
   char *item;
   unsigned int i;

   // free each item in our array and then free the array itself
   EINA_ARRAY_ITER_NEXT(array, i, item, iterator)
     free(item);
   eina_array_free(array);
}

static Eina_Bool
_item_print(const void *container EINA_UNUSED, void *data, void *fdata EINA_UNUSED)
{
   // we know our content is just strings so we can simply printf the data
   printf("  %s\n", (char *)data);
   return EINA_TRUE;
}

static Eina_Bool
_item_keep(void *data, void *gdata EINA_UNUSED)
{
   const char *name;
   name = (const char *)data;

   // let's keep any strings that are no more than 7 characters long
   if (strlen(name) <= 7)
      return EINA_TRUE;

   return EINA_FALSE;
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eina_Array *array;

   array = _array_create();

   // show the contents of our array
   printf("Array count: %d\n", eina_array_count(array));
   printf("Array contents:\n");
   eina_array_foreach(array, _item_print, NULL);

   // access a specific item in the array
   printf("Top gun: %s\n", (char*)eina_array_data_get(array, 2));

   // update a single item in the array
   eina_array_data_set(array, 17, strdup("flattop"));
   // update the array removing items that do not match the _item_keep criteria
   eina_array_remove(array, _item_keep, NULL);

   // print the new contents of our array
   printf("New array count: %d\n", eina_array_count(array));
   printf("New array contents:\n");
   eina_array_foreach(array, _item_print, NULL);

   _array_free(array);

   efl_exit(0);
}
EFL_MAIN()

