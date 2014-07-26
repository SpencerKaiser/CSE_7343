#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void goToSleep(int);

int main(int argc, char *argv[ ]) {
      int count;
      if (argc == 2)
            count = atoi(argv[1]);
      else
            count = 5;

      goToSleep(count);
}


void goToSleep(int seconds)
{
    int tick = 1;                                   //Initializes tick count for printing
    printf("%d; START\n", getpid());                //Prints initial PID and the 'Start' label

    while(seconds > 0)                              //Loop through 'seconds' number of times
    {
        sleep(1);                                   //Suspend calling process
        printf("%d; TICK %d\n", getpid(), tick);    //Print tick # and PID
        tick++;
        seconds--;
    }
}

//Does it always have the same process id?
  //No, but it makes sense that the PID changes each time the program is run because after
  //execution, the process does not simply restart, instead a new process is created
  //from the same program 

//What state do you think the process transitions to after a call to the sleep function?
  //I think the process moves to the 'Blocked' state when a function call to sleep() is made.
  //After the process begins to run and after sleep() is called, the process stops running and
  //waits until an event occurs, which seems consistent with the description of the 'Blocked' state.

//Server/Compiler Options
  //Server: genuse31
  //Compiler: gcc sleepy.c -o sleepy
  //I wrote and tested the program using Qt Creator which is a C/C++ IDE