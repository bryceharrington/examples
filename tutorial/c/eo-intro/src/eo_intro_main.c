#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>

#include <stdlib.h>

Eo *_root, *_child1, *_child2;

// Create our test hierarchy
static void
_obj_create()
{
   // First create a root element
   _root = efl_add(EFL_MODEL_ITEM_CLASS, NULL,
                   efl_name_set(efl_added, "Root"));

   // Create the first child element
   _child1 = efl_add(EFL_MODEL_ITEM_CLASS, _root,
                     efl_name_set(efl_added, "Child1"));

   // Create the second child element, this time, with an extra reference
   _child2 = efl_add_ref(EFL_MODEL_ITEM_CLASS, _root,
                         efl_name_set(efl_added, "Child2"));
}

// Destroy the test hierarchy
static void
_obj_destroy()
{
   // Destroy the root element
   printf ("Deleting Root...\n");
   efl_unref(_root);

   // Destroy the child2 element, for which we were keeping an extra reference
   printf ("Deleting Child2...\n");
   efl_unref(_child2);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   // Create all objects
   _obj_create();

   // Destroy all objects
   _obj_destroy();

   // Exit
   efl_exit(0);
}
EFL_MAIN()

