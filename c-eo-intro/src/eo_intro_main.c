#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>

#include <stdlib.h>

Eo *_root, *_child1, *_child2;
Eo *_root_ref, *_child1_ref, *_child2_ref;

// Retrieves the name of an object's parent, handling special cases like not
// having any parent, or the object having been already destroyed.
static const char*
_parent_name_get(Eo *obj)
{
   // Object has been destroyed
   if (efl_ref_get(obj) == 0) return "-";
   // Object has no parent
   if (efl_parent_get(obj) == NULL) return "none";
   // Otherwise, return parent's name
   return efl_name_get(efl_parent_get(obj));
}

// Prints status of all our objects in a pretty table
static void
_status_print()
{
   printf("Object:   %6s %6s %6s\n", "ROOT", "CHILD1", "CHILD2");
   printf("Refcount: %6d %6d %6d\n",
          efl_ref_get(_root_ref),
          efl_ref_get(_child1_ref),
          efl_ref_get(_child2_ref));
   printf("Parent:   %6s %6s %6s\n\n",
          _parent_name_get(_root_ref),
          _parent_name_get(_child1_ref),
          _parent_name_get(_child2_ref));
}

// Gets called whenever an object is deleted
static void
_obj_del_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   Eo *obj = event->object;

   printf("Object named \"%s\" is about to be deleted\n", efl_name_get(obj));
   _status_print();
}

// Create our test hierarchy
static void
_obj_create()
{
   // First create a root element
   _root = efl_add(EFL_MODEL_ITEM_CLASS, NULL,
                    efl_name_set(efl_added, "Root"));
   // Add a weak reference so we can keep track of its state
   efl_wref_add(_root, &_root_ref);
   // Register a callback for DELETION events
   efl_event_callback_add(_root, EFL_EVENT_DEL, _obj_del_cb, NULL);

   // Create the first child element
   _child1 = efl_add(EFL_MODEL_ITEM_CLASS, _root,
                    efl_name_set(efl_added, "Child1"));
   // Add a weak reference so we can keep track of its state
   efl_wref_add(_child1, &_child1_ref);
   // Register a callback for DELETION events
   efl_event_callback_add(_child1, EFL_EVENT_DEL, _obj_del_cb, NULL);

   // Create the second child element, this time, with an extra reference
   _child2 = efl_add_ref(EFL_MODEL_ITEM_CLASS, _root,
                        efl_name_set(efl_added, "Child2"));
   // Add a weak reference so we can keep track of its state
   efl_wref_add(_child2, &_child2_ref);
   // Register a callback for DELETION events
   efl_event_callback_add(_child2, EFL_EVENT_DEL, _obj_del_cb, NULL);
}

// Destroy the test hierarchy
static void
_obj_del()
{
   // Destroy the root element
   printf ("Deleting root...\n");
   efl_del(_root);
   printf ("After deleting root:\n");
   _status_print();

   // Destroy the child2 element, for which we were keeping an extra reference
   printf ("Deleting Child2...\n");
   efl_del(_child2);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   // Create all objects
   _obj_create();

   printf ("Initial state:\n");
   _status_print();

   // Destroy all objects
   _obj_del();

   printf ("Final state:\n");
   _status_print();

   // Exit
   efl_exit(0);
}
EFL_MAIN()

