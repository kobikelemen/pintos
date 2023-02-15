#include <stdio.h>
#include <syscall.h>
// #include "tests/lib.h"
// #include "tests/main.h"

int
main (int argc, char **argv)
{
  // int i;  
  // for (i = 1; i < argc; i++)
  //   printf ("%s ", argv[i]);
  // printf ("\n");

  printf ("here1\n");
  char sample[] = {
    "\"Amazing Electronic Fact: If you scuffed your feet long enough without" 
  };
  int handle, byte_cnt;

  bool success = create ("test.txt", sizeof sample - 1);
  if (success) 
    printf ("success\n");
  else  
    printf ("fail\n");
  handle = open ("test.txt");
  printf ("hndle: %i\n", handle);

  byte_cnt = write (handle, sample, sizeof sample - 1);
  printf ("AFTER write(), byte_cnt: %i\n", byte_cnt);
  char buf[byte_cnt];
  printf ("after defining buf\n");

  read (handle, buf, byte_cnt);
  printf ("buf: %s\n", buf);
  // if (byte_cnt != sizeof sample - 1)
  //   printf ("FAIL\n");
  // else  
  //   printf ("PASS\n");


  return EXIT_SUCCESS;
}
