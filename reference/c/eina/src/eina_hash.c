#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

/*
 * Eina Hash examples.
 *
 * These examples demonstrate how to work with eina_hash data and methods.
 *
 * We have two main hash objects here, firstly an int keyed hash with some
 * dummy content.
 * The second example is that of a simple telephone book. The names are the
 * string keys to the hash with phone numbers in the data field.
 */

static void
_entry_free_cb(void *data)
{
   free(data);
}

static Eina_Hash *
_hash_create()
{
   Eina_Hash *hash;
   unsigned int i;

   // let's create a simple hash with integers as keys
   hash = eina_hash_int32_new(_entry_free_cb);

   // Add initial entries to our hash
   for (i = 0; i < 10; i++)
     eina_hash_add(hash, &i, eina_slstr_printf("The number %d", i));

   return hash;
}

static void
_hash_demo()
{
   Eina_Hash *hash;
   Eina_Iterator *iter;
   unsigned int *key;
   const char *value;

   hash = _hash_create();

   // get an iterator of the keys so we can print a line per entry
   iter = eina_hash_iterator_key_new(hash);

   printf("Print contents of int hash\n");
   EINA_ITERATOR_FOREACH(iter, key)
   {
      // look up the value for the key so we can print both
      value = eina_hash_find(hash, key);
      printf("  Item found with id %d has value %s\n", *key, value);
   }
   eina_iterator_free(iter);
   printf("\n");

   eina_hash_free(hash);
}

// here we begin the phone book example

static void
_phonebook_entry_print(const void *key, void *data)
{
   printf("  Name: %s\tNumber %s\n", (const char *)key, (char *)data);
}

static void
_phonebook_print(Eina_Hash *book)
{
   Eina_Hash_Tuple *t;
   Eina_Iterator *iter;
   unsigned int count;

   count = eina_hash_population(book);
   printf("Complete phone book (%d):\n", count);

   // this iterator returns the key and value for each entry
   iter = eina_hash_iterator_tuple_new(book);
   EINA_ITERATOR_FOREACH(iter, t)
     _phonebook_entry_print(t->key, t->data);
   eina_iterator_free(iter);

   printf("\n");
}

static Eina_Hash *
_phonebook_create()
{
   const char *names[] =
   {
      "Wolfgang Amadeus Mozart", "Ludwig van Beethoven",
      "Richard Georg Strauss", "Heitor Villa-Lobos"
   };
   const char *numbers[] =
   {
      "+01 23 456-78910", "+12 34 567-89101",
      "+23 45 678-91012", "+34 56 789-10123"
   };

   Eina_Hash *hash;
   unsigned int i;

   // superfast is the type of hash, we could use others depending on key type
   hash = eina_hash_string_superfast_new(_entry_free_cb);

   // Add initial entries to our hash
   for (i = 0; i < 4; i++)
     eina_hash_add(hash, names[i], strdup(numbers[i]));

   return hash;
}

static void
_phonebook_demo()
{
   Eina_Hash *phone_book;
   const char *lookup_name = "Ludwig van Beethoven";
   char *number, *old_num;

   phone_book = _phonebook_create();
   _phonebook_print(phone_book);

   number = eina_hash_find(phone_book, lookup_name);
   printf("Found entry:\n");
   _phonebook_entry_print(lookup_name, number);
   printf("\n");

   // Let's first add a new entry
   eina_hash_set(phone_book, "Raul Seixas", strdup("+55 01 234-56789"));

   // Change the phone number for an entry
   old_num = eina_hash_set(phone_book, lookup_name, strdup("+12 34 222-22222"));
   if (old_num)
     printf("Old number for %s was %s\n\n", lookup_name, old_num);

   // Change the name (key) on an entry
   eina_hash_move(phone_book, "Raul Seixas", "Alceu Valenca");

   printf("After modifications\n");
   _phonebook_print(phone_book);

   eina_hash_free(phone_book);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   _hash_demo();
   _phonebook_demo();

   efl_exit(0);
}
EFL_MAIN()
