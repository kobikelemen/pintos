# pintos
My attempt at completing pintos!

Pintos is a small, basic operating system (OS) built by Stanford for learning. The goal of my project here is to add several core features of an OS to Pintos. These features include: a timer, a thread scheduler, running user programs, and implementing virtual memory.


## Thread Scheduling [Done]
Pintos comes with the functionality to change the thread that is executed by the CPU. But there is no way for multiple threads to run at the same time because there is no thread scheduler. In this part of the project I built a scheduler that decides when to change threads, and what thread to change to, such that all threads make progress (there is no starvation). This was achieved using a priority scheduler, that makes decisions on which thread to schedule based on its priority. A threads priority increases the longer is hasn't been scheduled. 

## User Programs [Done]
Up to here, all code that ran using Pintos was part of the OS kernel. A key part an OS is the ability to execute user made programs. This involved assigning the program an area of virtual memory, loading the user program from disk, setting the CPU registers, and scheduling the program for execution on the scheduler. The MMU was used to convert between the process' virtual memory and physical memory, raising a page fault if trying to access disallowed memory. Moreover, because you don't want to allow the user program free access to the kernel/ hardware, I implemented a number of system calls that the user program can user.

## Timer [Done]
The goal of this part is to improve the thread sleep functionality to switch to a new thread instead of busy waiting like it currently does.

## Virtual Memory [Done]
The final part is implementing virtual memory. Pintos currently splits main memory into pages, and gives each program an area of virtual memory that corresponds to these pages. However, the memory a program can use is limited by the size of main memory. To overcome this, pages that are assigned to a program but not currently in use can be swapped out to disk, and when needed they can be brought back to main memory. This means that programs can now use practically unlimited memory. The goal of this part of the project is to implement this.
