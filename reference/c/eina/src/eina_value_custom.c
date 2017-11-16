#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>
#include <sys/time.h>

#include <Eina.h>
#include <Efl_Core.h>

/*
 * Eina Value custom examples.
 *
 * Eina_Value allows the definition of custom types which can be passed like
 * any other Eina_Value. This example defines a custom timezone type and all
 * the methods that must be implemented for a custom type.
 * As this encapsulates the system timezone struct we initialise it from
 * the gettimeofday method..
 */

static Eina_Bool
_tz_setup(const Eina_Value_Type *type, void *mem)
{
   memset(mem, 0, type->value_size);
   return EINA_TRUE;
}

static Eina_Bool
_tz_flush(const Eina_Value_Type *type EINA_UNUSED, void *mem EINA_UNUSED)
{
   return EINA_TRUE;
}

static Eina_Bool
_tz_copy(const Eina_Value_Type *type EINA_UNUSED, const void *src, void * dst)
{
   struct timezone *tzsrc = (struct timezone *)src;
   struct timezone *tzdst = dst;
   *tzdst = *tzsrc;
   return EINA_TRUE;
}

static int
_tz_compare(const Eina_Value_Type *type EINA_UNUSED, const void *a, const void *b)
{
   struct timezone tza = *(struct timezone*)a;
   struct timezone tzb = *(struct timezone*)b;

   if (tza.tz_minuteswest < tzb.tz_minuteswest)
     return -1;
   else if (tza.tz_minuteswest > tzb.tz_minuteswest)
     return 1;
   return 0;
}

static Eina_Bool
_tz_pset(const Eina_Value_Type *type EINA_UNUSED, void *mem, const void *ptr)
{
   *(struct timezone*)mem = *(struct timezone*)ptr;
   return EINA_TRUE;
}

static Eina_Bool
_tz_vset(const Eina_Value_Type *type, void *mem, va_list args)
{
   const struct timezone tz = va_arg(args, struct timezone);
   return _tz_pset(type, mem, &tz);
}

static Eina_Bool
_tz_pget(const Eina_Value_Type *type, const void *mem, void *ptr)
{
   memcpy(ptr, mem, type->value_size);
   return EINA_TRUE;
}

static Eina_Bool
_tz_convert_to(const Eina_Value_Type *type EINA_UNUSED, const Eina_Value_Type *convert, const void *type_mem, void *convert_mem)
{
   struct timezone v = *(struct timezone*)type_mem;

   if (convert == EINA_VALUE_TYPE_UCHAR)
     {
        unsigned char val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_USHORT)
     {
        unsigned short val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_UINT)
     {
        unsigned int val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if ((convert == EINA_VALUE_TYPE_ULONG) || (convert == EINA_VALUE_TYPE_TIMESTAMP))
     {
        unsigned long val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_UINT64)
     {
        uint64_t val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_CHAR)
     {
        char val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_SHORT)
     {
        short val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_INT)
     {
        int val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_LONG)
     {
        long val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_INT64)
     {
        int64_t val_type = v.tz_minuteswest;
        return eina_value_type_pset(convert, convert_mem, &val_type);
     }
   else if (convert == EINA_VALUE_TYPE_FLOAT)
     return eina_value_type_pset(convert, convert_mem, &v.tz_minuteswest);
   else if (convert == EINA_VALUE_TYPE_DOUBLE)
     return eina_value_type_pset(convert, convert_mem, &v.tz_minuteswest);
   else if (convert == EINA_VALUE_TYPE_STRINGSHARE ||
            convert == EINA_VALUE_TYPE_STRING)
     {
        const char *buf;
        buf = eina_slstr_printf("+%03dh", v.tz_minuteswest);
        return eina_value_type_pset(convert, convert_mem, &buf);
     }
   return EINA_FALSE;
}

static Eina_Value_Type TZ_TYPE = {
   EINA_VALUE_TYPE_VERSION,
   sizeof(struct timezone),
   "struct timezone",
   _tz_setup,
   _tz_flush,
   _tz_copy,
   _tz_compare,
   _tz_convert_to,
   NULL, // No convert from
   _tz_vset,
   _tz_pset,
   _tz_pget
};

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eina_Value value_tz;
   struct timeval time;
   struct timezone zone;
   char *s;

   eina_value_setup(&value_tz, &TZ_TYPE);

   gettimeofday(&time, &zone);
   eina_value_set(&value_tz, zone);

   s = eina_value_to_string(&value_tz);
   printf("Current timezone: %s\n", s);
   free(s);

   eina_value_free(&value_tz);

   efl_exit(0);
}
EFL_MAIN()

