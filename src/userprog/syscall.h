#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "filesys/file.h"
#include <list.h>

/* Mapping from fd and file_name to actual file.
   List is part of struct thread. */
struct open_file
{
    struct file *f;
    int fd;
    char *name;
    struct list_elem elem;
};


void syscall_init (void);

#endif /* userprog/syscall.h */
