#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>
#include <Eina.h>

#include <Efl.h>
#include <Elementary.h>

static Eina_List *
_list_create()
{
   Eina_List *list = NULL;

   list = eina_list_append(list, "Adama");
   list = eina_list_append(list, "Baltar");
   list = eina_list_append(list, "Roslin");

   return list;
}

static void
_list_free(Eina_List *list)
{
   // here we do not need to use EINA_LIST_FREE as our data is not leaking
   eina_list_free(list);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eina_List *list, *item, *sorted;
   void *item_data;

   list = _list_create();

   // print our list with a simple foreach
   printf("List size: %d\n", eina_list_count(list));
   printf("List content:\n");
   EINA_LIST_FOREACH(list, item, item_data)
     printf("  %s\n", (char*)item_data);

   // insert some more elements
   list = eina_list_prepend(list, "Cain");
   list = eina_list_prepend_relative(list, "Tigh", "Baltar");

   printf("New list content:\n");
   EINA_LIST_FOREACH(list, item, item_data)
     printf("  %s\n", (char*)item_data);

   // promote an item to the top of the list
   list = eina_list_promote_list(list, eina_list_nth_list(list, 1));
   list = eina_list_remove(list, "Cain");

   printf("List content after promotion:\n");
   EINA_LIST_FOREACH(list, item, item_data)
     printf("  %s\n", (char*)item_data);

   // we can sort the list with any callback, here we use strcmp
   sorted = eina_list_sort(list, 0, (Eina_Compare_Cb)strcmp);
   printf("List content sorted:\n");
   EINA_LIST_FOREACH(sorted, item, item_data)
     printf("  %s\n", (char*)item_data);

   // and foreach can be in reverse too
   printf("List content reverse sorted:\n");
   EINA_LIST_REVERSE_FOREACH(sorted, item, item_data)
     printf("  %s\n", (char*)item_data);

   _list_free(list);

   efl_exit(0);
}
EFL_MAIN()
