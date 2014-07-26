#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void goToSleep(int, int);
int status;

int main(int argc, char *argv[ ]) {
      int count;
      if (argc == 2)
            count = atoi(argv[1]);
      else
            count = 5;

      pid_t childPID = fork();
      //In the child process, this value will be 0
      //Using this value, I will test to see
      //whether the current process is the parent
      //or child so I can label the output accordingly.

      if(childPID == 0)                         //CHILD PROCESS
      {
          goToSleep(count, childPID);
          exit(1);                                  //At end of execution, exit so parent process can execute
      }
      else                                      //PARENT PROCESS
      {
          wait(&status);                            //Wait until child process completes and exits
          goToSleep(count, childPID);
      }
}


void goToSleep(int seconds, int childPID)
{
    char* processType;
    int tick = 1;                                   //Initializes tick count for printing

    if(childPID == 0)                               //If value is 0, process is the child
        processType = "Child";
    else
        processType = "Original";                   //If not, process is the parent


    while(seconds > 0)                              //Loop through 'seconds' number of times
    {
        sleep(1);                                   //Suspend calling process

                                                    //Print the processType, PID, PPID, and tick #
        printf("%s Process with PID: %d and PPID: %d TICK %d\n", processType, getpid(), getppid(), tick);
        tick++;
        seconds--;
    }
}

/* After commenting out the function calls to sleep(), there was no change in the ordering of the output.
 * When the function call to wait() is made, it forces the parent process to wait until the child process
 * exits execution, so either way, the child process will run and print the output before the parent
 * process will get its turn.
 */

//Server/Compiler Options
  //Server: genuse31
  //Compiler: gcc twoSleepy.c -o twoSleepy
  //I wrote and tested the program using Qt Creator (C/C++ IDE) before
    //compiling and running the program on the genuse server

