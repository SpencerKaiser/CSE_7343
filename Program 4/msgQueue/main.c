#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>


void producer(int, int, int);
void consumers(int, int, int);

typedef struct {
    int numItems, threadNum, msqid, msg_size;
} thread_args;

struct rand_msgbuf {
    long mtype;
    int rand;
};

void *threadFunc(void* args) {
    thread_args* arguments = args;
    int numItems = arguments->numItems, threadNum = arguments->threadNum, msqid = arguments->msqid, msg_size = arguments->msg_size;
    struct rand_msgbuf incomingMSG = {1,0};

    printf("Thread %d Created\n", threadNum);

    int C, total = 0;

    for(int i = 0; i < numItems; i++)
    {
//        printf("\tIn loop # %d of %d\n", i + 1, numItems);

        msgrcv(msqid, &incomingMSG, msg_size, 1, 0);
        C = incomingMSG.rand;                                               //1)    Read a message containing a value, C, consumed from the queue.

        total += C;                                                         //2)    Add C to a running total maintained by each consumer thread.

        printf("-\tConsumer thread %d consumed a %d\n", threadNum, C);        //3)    Display the string “Consumer thread Z consumed a C” where Z is the thread number – 0 or 1.

        sleep(rand()%4 + 1);                                                //4)    Put the consumer thread to sleep for 1-4 seconds using sleep((rand()%4)+1)
    }

    printf("Total consumed by consumer thread %d = %d\n", threadNum, total);
    printf("Terminating Thread %d\n", threadNum);

    pthread_exit(NULL);
}


int main(int argc, char *argv[ ]) {
    printf("\n\n\nWelcome to Program 4\n\n\n");
    int numItems;
    if (argc == 2)
        numItems = atoi(argv[1]);
    else
        numItems = 5;                                                       //Set default value to 5

    srand(time(NULL));                                                      //INITIALIZE RAND SEED
    key_t key = ftok("", 'a');                                              //Creates a key of 0xffffffff
    int msqid = msgget(key, 0666 | IPC_CREAT);                              //CREATE MESSAGE QUEUE
    int msg_size = sizeof(int);

    pid_t childPID = fork();                                                //CREATE CHILD PROCESS (childPID will be 0 in the child process)

    if(childPID == 0)                                                       //CHILD PROCESS
        consumers(numItems, msqid, msg_size);
    else                                                                    //PARENT PROCESS
        producer(numItems, msqid, msg_size);

    printf("DELETING MESSAGE QUEUE @ %d\n\n", msqid);
    msgctl(msqid, IPC_RMID, NULL);                                          //DELETE MESSAGE QUEUE
    printf("Thank you for using Program 4\n\n\n");
}



void producer(int numItems, int msqid, int msg_size) {
    printf("Producer Created\n");
    int R, total = 0, status;

    struct rand_msgbuf outgoingMSG = {1,0};

    for(int i = 0; i < numItems; i++)                                       //ITERATE numItems TIMES
    {
        R = rand()%1000;                                                    //1)    Generate a random number, R, in the range of 0-999.

        outgoingMSG.rand = R;                                               //2a)    Prepare message
        msgsnd(msqid, &outgoingMSG, msg_size, 0);                           //2b)    Send a message containing R.
        //Q: If the message is passed by reference, why is the original message not overwritten when the next message is sent??

        total += R;                                                         //3)    Add R to a running total of all values produced.

        printf("+\tProducer produced a %d\n", R);                           //4)    Display the string “Producer produced a R”.

        sleep(rand()%2 + 1);                                                //5)    Put the producer to sleep for 1-2 seconds using sleep((rand()%2)+1).
    }

    wait(&status);                                                          //Wait until child process completes and exits

    printf("Total Produced: %d\n\n", total);
}

void consumers(int numItems, int msqid, int msg_size) {
                                                                            //ENTER CHILD PROCESS
    int numThreads = 2, threadOneNumItems, threadTwoNumItems;

    if(numItems %2 == 0)                                                    //numItems is even
        threadOneNumItems = threadTwoNumItems = numItems/2;
    else                                                                    //numItems is odd; thread two will have +1 cycles
    {
        threadOneNumItems = numItems/2;
        threadTwoNumItems = threadOneNumItems + 1;
    }

    //PREPARE THREAD ARGUMENTS
    thread_args threadOneArgs = {threadOneNumItems, 0, msqid, msg_size};
    thread_args threadTwoArgs = {threadTwoNumItems, 1, msqid, msg_size};

    thread_args threadArgs[numThreads];
    threadArgs[0] = threadOneArgs;
    threadArgs[1] = threadTwoArgs;


    //CREATE ARRAY TO HOLD THREADS
    pthread_t threads[numThreads];

    pthread_attr_t attr;                                                    //Declare a pthread attribute variable of the pthread_attr_t data type
    pthread_attr_init(&attr);                                               //Initialize the attribute variable with pthread_attr_init()
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);            //Set the attribute detached status with pthread_attr_setdetachstate()

    //CREATE THREADS
    for(int i = 0; i < numThreads; i++)
        pthread_create(&threads[i], &attr, threadFunc, &threadArgs[i]);

    pthread_attr_destroy(&attr);                                            //Free library resources used by the attribute with pthread_attr_destroy()

    //JOIN THREADS
    for(int i = 0; i < numThreads; i++)
        pthread_join(threads[i], NULL);

    exit(1);                                                                //EXIT CHILD PROCESS
}


/*
 * MUTUAL EXCLUSION: Mutual Exclusion is achieved through the message send and receive functions.
 * When a call to msgrcv() is made, the thread calling it is put in a blocked state to prevent
 * it from trying to read a message from the queue when none exist. Once a message is added to the
 * queue, a single thread is allowed to enter its critical section (reading a message from the queue)
 * which blocks other threads from entering their critical section. Once a thread has read and consumed
 * a message, it exits the critical section and allows others to enter. At this point, if the producer
 * has generated another message, the other thread will have an opportunity to enter it's critical
 * section. If not, it will be blocked and the two threads will continue to try to enter until a new
 * message is added.
 *
 *
 * SYNCHRONIZATION: Similar to the above, the msgrv() function is a blocking function so the producer/
 * consumer problem is solved by preventing consumers from trying to consume until there is an item in
 * the message queue. This ensures synchronization amongst processes.
 *
 */

