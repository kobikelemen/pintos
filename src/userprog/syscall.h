#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "filesys/file.h"
#include "filesys/filesys.h"
#include "filesys/directory.h"
#include "filesys/inode.h"
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

void open_file_init (struct open_file *_open_file, struct file *_f, 
                     int _fd, char *_name);
void syscall_init (void);

#endif /* userprog/syscall.h */
