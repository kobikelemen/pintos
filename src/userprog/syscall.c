#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/palloc.h"

static void syscall_handler (struct intr_frame *);
struct file *file_from_fd (int fd);
int openfiles_add (struct file *f, char *file_name);


void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


bool sys_create (const char *file_name, unsigned initial_size)
{
  return filesys_create (file_name, initial_size);
}

/* Opens file with name file_name and returns the file 
   descriptor (fd). */
int sys_open (const char *file_name)
{
  struct inode *_inode;
  struct file *f = filesys_open (file_name);
  if (f) 
    {
      /* Add file to current threads open file list */
      int fd = openfiles_add (f, file_name);
      return fd;
    }

  return -1;
}


int sys_read (int fd, void *buffer, unsigned size)
{
  struct file *f = file_from_fd (fd);
  if (f) {
    off_t bytes_read = file_read (f, buffer, size);
    return bytes_read;
  }
  return -1;
}


void sys_close (int fd)
{

}

int sys_write (int fd, char *buffer, unsigned size)
{
  if (fd == 1) 
    {
      putbuf ((void*)buffer, size);
      return;
    }
  struct file *f = file_from_fd (fd);
  int bytes_written = 0;
  if (f)
    bytes_written = file_write (f, buffer, size);
  return bytes_written;
}


void sys_exit (int status)
{
  printf ("%s: exit(%d)\n", thread_current ()->name, status);
  thread_exit ();
}



void openfile_init (struct open_file *openfile, struct file *_f, 
                     int _fd, char *_name)
{
  openfile->f = _f;
  openfile->fd = _fd;
  openfile->name = _name;
}


/* Creates open_file struct and adds it to current threads
   open file list. Returns file descriptor. */
int openfiles_add (struct file *f, char *file_name)
{
  
  struct thread *cur = thread_current ();
  int fd = openfiles_new_fd (&cur->open_files, file_name);
  if (fd == -1)
    return -1;

  struct open_file *openfile = palloc_get_page (PAL_ZERO);

  openfile_init (openfile, f, fd, file_name);
  list_push_back (&cur->open_files, &openfile->elem);
  return fd;
}

/* Loops through each open_file in open_files to check that 
   file_name is not already present. If not, returns first 
   available file descriptor (fd). */
int openfiles_new_fd (struct list open_files, char *file_name)
{
  struct thread *t = thread_current ();
  struct list_elem *e;
  int max = 1;

  for (e = list_begin (&t->open_files); e != list_end (&t->open_files);
       e = list_next (e))
    {
      struct open_file *of = list_entry (e, struct open_file, elem);
      if (strcmp (of->name, file_name) == 0) 
        return -1;
      if (of->fd > max) 
        max = of->fd;
    }

  return max + 1;
}


struct file *file_from_fd (int fd)
{
  struct thread *t = thread_current ();
  struct list_elem *e;
  // printf ("in file_from_fd()\n");

  for (e = list_begin (&t->open_files); e != list_end (&t->open_files);
       e = list_next (e))
    {
      struct open_file *of = list_entry (e, struct open_file, elem);
      if (of->fd == fd) {
        // printf ("file name in file_from_fd(): %s\n", of->name);
        return of->f;
      }
    }
  return NULL;
}




static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  // printf ("system call!\n");
  int *stack_ptr = f->esp;
  int syscall_num = *stack_ptr;
  
  if (syscall_num == SYS_WRITE) 
    {
      int arg0 = *(stack_ptr + 1); // fd
      char *arg1 = (char*)(*(stack_ptr + 2)); // buffer
      int arg2 = *(stack_ptr + 3); // size
      int bytes_written = sys_write (arg0, arg1, arg2); 
      f->eax = bytes_written;
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
  else if (syscall_num == SYS_CREATE) 
    {
      char *arg0 = (char*)(*(stack_ptr + 1)); // file name
      int arg1 = *(stack_ptr + 2); // size
      bool success = sys_create (arg0, arg1);
      f->eax = success;
    }
  else if (syscall_num == SYS_OPEN)
    {
      char *arg0 = (char*)(*(stack_ptr + 1)); // file name
      int fd = sys_open (arg0);
      f->eax = fd;
    }
  else if (syscall_num == SYS_READ)
    {
      int arg0 = *(stack_ptr + 1); // fd
      char *arg1 = (char*)(*(stack_ptr + 2)); // buffer
      int arg2 = *(stack_ptr + 3); // size
      off_t bytes_read = sys_read (arg0, arg1, arg2); 
      f->eax = bytes_read;
    }
        
}