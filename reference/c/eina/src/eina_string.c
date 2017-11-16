#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

static void
_string_splitjoin()
{
   const char *names = "Calvin;Leoben;D'anna;Simon;Doral;Six;Daniel;Sharon";
   char **arr;
   char *joined;
   int i;

   printf("Name list split\n");
   arr = eina_str_split(names, ";", 0);
   for (i = 0; arr[i]; i++)
     printf(" %s\n", arr[i]);

   joined = malloc(sizeof(char) * 11);
   eina_str_join_len(joined, 11, '&', arr[0], strlen(arr[0]),
                     arr[1], strlen(arr[1]));
   printf("Joined limit %s\n", joined);

   free(joined);
   // To free this array we only need to free the first item and the array
   free(arr[0]);
   free(arr);
}

static void
_string_case()
{
   char *str;

   str = strdup("Welcome");
   printf("String: %s\n", str);

   eina_str_toupper(&str);
   printf(" Upper: %s\n", str);
   eina_str_tolower(&str);
   printf(" Lower: %s\n", str);

   free(str);
}

static void
_string_startend()
{
   const char *file = "libefl.so";
   char *start;

   printf("File named %s:\n", file);
   printf(" starts with \"lib\"? %d\n", eina_str_has_prefix(file, "lib"));
   printf(" ends with \"efl.SO\"? %d\n", eina_str_has_suffix(file, "efl.SO"));
   printf(" has extension \".SO\"? %d\n", eina_str_has_extension(file, ".SO"));

   start = malloc(sizeof(char) * 7);
   eina_strlcpy(start, file, 7);
   printf(" first 6 chars \"%s\"\n", start);
   free(start);
}

static void
_string_escape()
{
   const char *str;
   char *tmp;

   str = "Here's some \"example\" text.";
   tmp = eina_str_escape(str);
   printf("String: %s\n", str);
   printf("Escaped: %s\n", tmp);
   free(tmp);
}

static void
_string_time()
{
   char *time_arr;
   time_t curr_time;
   struct tm *info;

   curr_time = time(NULL);
   info = localtime(&curr_time);
   time_arr = eina_strftime("%d/%m/%Y", info);
   printf("Today's Date: %s\n", time_arr);
   free(time_arr);
}

static void
_string_share()
{
   const char *str;
   const char *line1 = "The Cylons were created by man. They rebelled. They "
                       "evolved.";
   const char *line2 = "%d Cylon models. %d are known. %d live in secret. "
                       "%s will be revealed.";
   const char *line3 = "There are many copies. And they have a plan.";

   printf("Line1: %s\n", line1);
   str = eina_stringshare_add_length(line1, 31);
   printf(" limited to %d: %s\n", eina_stringshare_strlen(str), str);
   eina_stringshare_del(str);

   str = eina_stringshare_printf(line2, 12, 3, 4, "four");
   printf("Line2: %s\n", str);
   eina_stringshare_del(str);
   str = eina_stringshare_nprintf(48, line2, 12, 7, 5);
   printf(" format limit (len %d): %s...\n", eina_stringshare_strlen(str), str);
   eina_stringshare_del(str);

   printf("Line3: %s\n", line3);
   str = eina_stringshare_add(line3);
   printf(" shared: %s\n", str);
   eina_stringshare_replace(&str, line1);
   printf(" replaced: %s\n", str);
   eina_stringshare_del(str);
}

static void
_string_buf()
{
   Eina_Strbuf *buf, *substr;

   buf = eina_strbuf_new();

   eina_strbuf_append_length(buf, "BUFFE", 5);
   eina_strbuf_append_char(buf, 'R');
   printf("buffer: %s\n", eina_strbuf_string_get(buf));

   eina_strbuf_tolower(buf);
   printf("lower: %s\n", eina_strbuf_string_get(buf));

   substr = eina_strbuf_substr_get(buf, 3, 2);
   printf("substr: %s\n", eina_strbuf_string_get(substr));
   eina_strbuf_free(substr);

   eina_strbuf_insert_escaped(buf, "my ", 0);
   printf("escaped: %s\n", eina_strbuf_string_get(buf));
   eina_strbuf_reset(buf);
   printf("\n");

   eina_strbuf_append_printf(buf, "%s%c", "buffe", 'r');
   eina_strbuf_insert_printf(buf, " %s: %d", 6, "length", (int)eina_strbuf_length_get(buf));
   printf("printf: %s\n", eina_strbuf_string_get(buf));

   eina_strbuf_remove(buf, 7, 15);
   printf("removed: %s\n", eina_strbuf_string_get(buf));

   eina_strbuf_replace_all(buf, "buffer", "size");
   printf("replaced: %s\n", eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);
   printf("\n");

   // Let's manage a new buffer with a readonly string (i.e. not malloc'd)
   buf = eina_strbuf_manage_read_only_new_length("Example string", 14);
   printf("manage: %s\n", eina_strbuf_string_get(buf));
   // We can still mutate as internals deal with readonly -> write
   eina_strbuf_append_char(buf, '.');
   printf("append: %s\n", eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
}

static void
_string_buf_time()
{
   Eina_Strbuf *buf;
   time_t curr_time;
   struct tm *info;

   curr_time = time(NULL);
   info = localtime(&curr_time);

   buf = eina_strbuf_new();
   eina_strbuf_append_strftime(buf, "%I:%M%p", info);
   printf("current time: %s\n", eina_strbuf_string_get(buf));
   eina_strbuf_reset(buf);

   eina_strbuf_append(buf, "Hours, Minutes");
   // Add hours at the beginning
   eina_strbuf_prepend_strftime(buf, "%I ", info);
   // Insert minutes at the right position
   eina_strbuf_insert_strftime(buf, "%M ", info, 10);
   printf("%s\n", eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   _string_splitjoin();
   printf("\n");

   _string_case();
   printf("\n");

   _string_startend();
   printf("\n");

   _string_escape();
   printf("\n");

   _string_time();
   printf("\n");

   _string_share();
   printf("\n");

   _string_buf();
   printf("\n");
   _string_buf_time();

   efl_exit(0);
}
EFL_MAIN()

