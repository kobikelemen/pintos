#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);
struct file *file_from_fd (int fd);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void sys_write (int fd, char *buffer, unsigned size)
{
  if (fd == 1) 
    {
      putbuf ((void*)buffer, size);
      return;
    }
  struct file *f = file_from_fd (fd);
  if (f)
    file_write (f, buffer, size);
}


void sys_exit (int status)
{
  printf("exit status: %i\n", status);
  /* TODO: Return status in eax register 
     (in intr_frame, syscall_handler arg) */
  thread_exit ();
}


static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  int *stack_ptr = f->esp;
  int syscall_num = *stack_ptr;
  
  if (syscall_num == SYS_WRITE) 
    {
      int arg0 = *(stack_ptr + 1); // fd
      /* Count size of arg1 (buffer). */
      int str_size = 0;
      char *arg1 = (char*)(*(stack_ptr + 2)); // buffer
      int arg2 = *(stack_ptr + 3); // size
      sys_write (arg0, arg1, arg2); 
    }  
  else if (syscall_num ==  SYS_EXIT) 
    {
      int arg0 = *(stack_ptr + 1);
      if (arg0 == 0) 
        f->eax = 0; /* Return 0 = success. */
      else 
        f->eax = 1; /* Return 1 = fail. */
      sys_exit (arg0);
    }
        
}



struct file *file_from_fd (int fd)
{
  struct thread *t = thread_current ();
  struct list_elem *e;

  for (e = list_begin (&t->open_files); e != list_end (&t->open_files);
       e = list_next (e))
    {
      struct open_file *of = list_entry (e, struct open_file, elem);
      if (of->fd == fd) {
        return of->f;
      }
    }

  return NULL;
}