#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1

#include <stdio.h>

#include <Eina.h>
#include <Efl_Core.h>

#define FILENAME "/tmp/core_loop_test.txt"

/*
 * Efl Core Loop examples.
 *
 * TODO
 */

static int _count = 0;
static void _loop_fd_read();

static void
_print_loop(Efl_Loop *loop, const char *label)
{
   printf("%s loop found at %p \"%s\"\n", label, loop, efl_name_get(loop));
}

static void
_loop_current(Efl_Loop *current)
{
   Efl_Loop *loop;

   loop = current;
   _print_loop(loop, "Current");
   loop = efl_loop_main_get(current);
   _print_loop(loop, "Application Main");

   loop = efl_add(EFL_LOOP_USER_CLASS, NULL,
                  efl_name_set(efl_added, "User Loop"));
   _print_loop(loop, "Child");
   efl_del(loop);
}

static void
_loop_fd_read_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   Efl_Loop_Fd *fd;
   char buf[7];
   int len;

   fd = event->object;

   len = read(efl_loop_fd_file_get(fd), &buf, sizeof(buf));

   // here we are exiting
   if (len <= 0)
     {
        efl_del(fd);
        unlink(FILENAME);

        efl_exit(0);
        return;
     }

   buf[len] = 0;
   printf("READ %s", buf);
}

static void
_loop_fd_write_cb(void *data EINA_UNUSED, const Efl_Event *event)
{
   Efl_Loop_Fd *fd;

   fd = event->object;

   // we have outputted all we want to, remove the write handler
   // start checking for read availability instead
   if (_count >= 5)
     {
        efl_del(fd);

        _loop_fd_read();
        return;
     }

   _count++;
   printf("WRITING %d\n", _count);
   write(efl_loop_fd_file_get(fd), eina_slstr_printf("TEST %d\n", _count), 7);
}

static void
_loop_fd_write()
{
   Efl_Loop_Fd *loop_fd;
   FILE *file;
   int fd;

   loop_fd = efl_add(EFL_LOOP_FD_CLASS, NULL,
                  efl_name_set(efl_added, "Write Loop"));

   efl_event_callback_add(loop_fd, EFL_LOOP_FD_EVENT_WRITE, _loop_fd_write_cb, NULL);

   file = fopen(FILENAME, "w+");
   fd = fileno(file);

   printf("Opened file with fd %d\n", fd);
   efl_loop_fd_file_set(loop_fd, fd);

}

static void
_loop_fd_read()
{
   Efl_Loop_Fd *loop_fd;
   FILE *file;
   int fd;

   loop_fd = efl_add(EFL_LOOP_FD_CLASS, NULL,
                     efl_name_set(efl_added, "Read Loop"));

   efl_event_callback_add(loop_fd, EFL_LOOP_FD_EVENT_READ, _loop_fd_read_cb, NULL);

   file = fopen("/tmp/core_loop_test.txt", "r");
   fd = fileno(file);

   printf("Opened file with fd %d\n", fd);
   efl_loop_fd_file_set(loop_fd, fd);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev)
{
   const Efl_Version *version;

   version = efl_loop_efl_version_get(ev->object);
   printf("Running on EFL version %d.%d.%d [%s]\n", version->major, version->minor,
                                                    version->micro, version->build_id);
   printf("\n");

   _loop_current(ev->object);
   printf("\n");

   _loop_fd_write();
   // we will call _loop_fd_read() once write is complete!

   // we will exit from the end of the read loop
}
EFL_MAIN()

