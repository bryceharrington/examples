#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <Eina.h>
#include <Efl.h>
#include <Elementary.h>

#include <stdlib.h>

Eo *_ref1, *_ref2;

static Eo *
_obj_create()
{
   Eo *obj, *child;
   Eina_Bool finalized;

   obj = efl_add(EFL_MODEL_ITEM_CLASS, NULL,
                 efl_name_set(efl_added, "Parent Object"),
                 finalized = efl_finalized_get(efl_added));
   printf("Was the parent final? %d\n", finalized);
   finalized = efl_finalized_get(obj);
   printf("Is the parent final now? %d\n", finalized);

   child = efl_add(EFL_MODEL_ITEM_CLASS, obj,
                   efl_name_set(efl_added, "Child Object 1"));
   efl_wref_add(child, &_ref1);
   child = efl_add_ref(EFL_MODEL_ITEM_CLASS, obj,
                       efl_name_set(efl_added, "Child Object 2"));
   efl_wref_add(child, &_ref2);

   printf("Child reference count %d, %d\n", efl_ref_get(_ref1), efl_ref_get(_ref2));
   return obj;
}

static void
_obj_callback_cb(void *data, const Efl_Event *event)
{
   Eo *obj = data;

   printf("Callback %s on object named \"%s\"\n", event->desc->name, efl_name_get(obj));
}

static void
_obj_del_cb(void *data, const Efl_Event *event EINA_UNUSED)
{
   Eo *obj = data;

   printf("Object named \"%s\" deleted\n", efl_name_get(obj));
}

static void
_obj_callbacks(Eo *obj)
{
   efl_event_callback_add(obj, EFL_EVENT_CALLBACK_ADD, _obj_callback_cb, obj);
   efl_event_callback_add(obj, EFL_EVENT_CALLBACK_DEL, _obj_callback_cb, obj);
   efl_event_callback_add(obj, EFL_EVENT_DEL, _obj_del_cb, obj);

   efl_event_callback_del(obj, EFL_EVENT_CALLBACK_ADD, _obj_callback_cb, obj);
}

static void
_obj_del(Eo *parent)
{
   efl_del(parent);
   printf("New child refcount %d, %d\n", efl_ref_get(_ref1), efl_ref_get(_ref2));

   efl_del(_ref2);
   printf("New child refcount %d, %d\n", efl_ref_get(_ref1), efl_ref_get(_ref2));
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eo* obj;

   obj = _obj_create();
   printf("Object name %s\n", efl_name_get(obj));

   _obj_callbacks(obj);
   _obj_del(obj);

   efl_exit(0);
}
EFL_MAIN()

