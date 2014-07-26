#include <stdio.h>
#include <stdlib.h>
#include <time.h>
    #include <pthread.h>

void arrayManager(int);                                                 //Manager for creation and sorting
void fillArray(int, int*);                                              //Use SRAND and RAND to fill array
void copyArray(int, int*, int*);                                        //Duplicate an array
void printArray(int, int*);                                             //Print contents of array
double sortArray(int, int*);                                            //Sort array and return elapsed time
double mergeArrays(int, int*, int*, int*);                              //Merge two arrays and return elapsed time

void pthreadSort(int, int*, int*, int*, double[2]);                     //Use PTHREADS to sort 2 arrays concurrently and merge into a double-length array
void sortAndMerge(int, int*, int*, int*, double[2]);                    //Sort 2 arrays sequentially, then merge into a double-length array
double sortDouble(int, int*);                                           //Sort one double-length array

//Struct to allow threads to pass multiple arguments
typedef struct thread_args {
    int numItems;
    int* array;
    double sortTime;
    int threadNum;
} thread_args;

//Thread function to sort arrays using a struct containing relevant arguments
void *threadSort(void* args) {
    thread_args* arguments = args;
    printf("\nIN THREAD %d\n", arguments->threadNum);
    arguments->sortTime = sortArray(arguments->numItems, arguments->array);
    printf("\n\tThread %d time: %.5f", arguments->threadNum, arguments->sortTime);
    pthread_exit(NULL);
}



int main(void) {
    int numItems = 1;                                                   //Initializing numItems to start loop

    while(numItems != 0)
    {
        printf("\n\nPlease enter an array size or enter '0' to exit the program: ");

        while (scanf("%d", &numItems) != 1)                             //Make sure an INT was entered
        {
            while (getchar() != '\n');
            printf ("\n\nINVALID SELECTION... Please try again...: ");
        }

        if(numItems == 0)                                               //User wants to exit program
        {
            printf("\n\nExiting the program. Goodbye!\n\n");
            exit(1);
        }
        else if (numItems > 0)                                          //A valid INT was entered
            arrayManager(numItems);
        else                                                            //An invalid INT was entered
            printf("\n\nINVALID SELECTION. Please try again...\n");
    }
}


void arrayManager(int numItems) {

    //CREATE ARRAYS AND ALLOCATE MEMORY
    int* arrayOneOriginal = (int*) malloc(numItems*sizeof(int));        //Original array of length 'numItems'
    int* arrayTwoOriginal = (int*) malloc(numItems*sizeof(int));        //Original array of length 'numItems'
    int* pthreadsArray = (int*) malloc(numItems*sizeof(int)*2);         //Double-length array for results of pthreded sort

    int* arrayOneCopy = (int*) malloc(numItems*sizeof(int));            //Original array of length 'numItems'
    int* arrayTwoCopy = (int*) malloc(numItems*sizeof(int));            //Original array of length 'numItems'
    int* mergedArray = (int*) malloc(numItems*sizeof(int)*2);           //Double-length array for results of standard sort and merge

    int* doubleArray = (int*) malloc(numItems*sizeof(int)*2);           //Double-length array of length 2 * 'numItems'


    //INITIALIZE SRAND SEED
    srand(time(NULL));


    //FILL ORIGINAL ARRAYS
    fillArray(numItems, arrayOneOriginal);
    fillArray(numItems, arrayTwoOriginal);


    //CREATE COPY ARRAYS
    copyArray(numItems, arrayOneOriginal, arrayOneCopy);
    copyArray(numItems, arrayTwoOriginal, arrayTwoCopy);


    //CREATE DOUBLE-LENGTH COPY
    int i;
    for(i = 0; i < numItems; i++)                                   //Fill first half of double-length array
        doubleArray[i] = arrayOneOriginal[i];
    for(i = 0; i < numItems; i++)
        doubleArray[i + numItems] = arrayTwoOriginal[i];                //Fill second half of double-length array


    //CREATE SUMMARY ARRAYS TO HOLD TIMING DATA
    double pthreadSummary[3];
    double sortAndMergeSummary[2];
    double doubleLengthSortTime = 0;


    //SORT & MERGE
    sortAndMerge(numItems, arrayOneCopy, arrayTwoCopy, mergedArray, sortAndMergeSummary);
    pthreadSort(numItems, arrayOneOriginal, arrayTwoOriginal, pthreadsArray, pthreadSummary);
    doubleLengthSortTime = sortDouble(numItems*2, doubleArray);


    //PRINT SUMMARY
    printf("\n\n\nCONCURRENT SORTING & MERGE SUMMARY\n\n");
    printf("\t\t Sort Time: %.5f\n\t\tMerge Time: %.5f\n\tTOTAL ELAPSED TIME: %.5f",pthreadSummary[0], pthreadSummary[2], pthreadSummary[0] + pthreadSummary[2]);

    printf("\n\n\nSEQUENTIAL SORTING & MERGE SUMMARY\n\n");
    printf("\t\t Sort Time: %.5f\n\t\tMerge Time: %.5f\n\tTOTAL ELAPSED TIME: %.5f",sortAndMergeSummary[0], sortAndMergeSummary[1], sortAndMergeSummary[0] + sortAndMergeSummary[1]);

    printf("\n\n\nDOUBLE-LENGTH SORTING SUMMARY\n\n");
    printf("\tTOTAL ELAPSED TIME: %.5f", doubleLengthSortTime);


    //FREE ALLOCATED MEMORY
    free(arrayOneOriginal);
    free(arrayTwoOriginal);
    free(pthreadsArray);

    free(arrayOneCopy);
    free(arrayTwoCopy);
    free(mergedArray);

    free(doubleArray);
}

void fillArray(int numItems, int* array) {
    int i;
    for(i = 0; i < numItems; i++)
        array[i] = rand() % 1000;                                       //Generate random number between 0 and 999
}

void copyArray(int numItems, int* source, int* dest) {
    int i;
    for(i = 0; i < numItems; i++)
        dest[i] = source[i];
}

void printArray(int numItems, int* array) {
    int i;
    for(i = 0; i < numItems; i++)
        printf("%d  ", array[i]);
    printf("\n");
}



double sortArray(int numItems, int* array) {
    clock_t elapsedTime;

    elapsedTime = clock();                                              //Start timer

    int x, y;

    //BUBBLE SORT
    for(x = 0; x < numItems; x++)
    {
        for(y = 0; y < numItems - 1; y++)
        {
            if(array[y] > array[y+1])
            {
                int temp = array[y + 1];
                array[y + 1] = array[y];
                array[y] = temp;
            }
        }
    }

    elapsedTime = clock() - elapsedTime;                                //End timer
    return ((float)(elapsedTime)/CLOCKS_PER_SEC);                       //Return duration (seconds)
}



double mergeArrays(int numItems, int* arrayOne, int* arrayTwo, int* mergedArray) {
    clock_t elapsedTime;
    elapsedTime = clock();                                              //Start timer

    int locationOne = 0;        //Index of arrayOne
    int locationTwo = 0;        //Index of arrayTwo

    int i;
    for(i = 0; i < numItems*2; i++)
    {
        if(locationOne < numItems && locationTwo < numItems)            //If both arrays have remaining values, add smallest to mergedArray
        {
            if(arrayOne[locationOne] < arrayTwo[locationTwo])
            {
                mergedArray[i] = arrayOne[locationOne];
                locationOne++;
            }
            else
            {
                mergedArray[i] = arrayTwo[locationTwo];
                locationTwo++;
            }
        }
        else if(locationOne < numItems)                                 //If arrayTwo is exhausted...
        {
            while(i < numItems*2)                                       //Add all remaining items from arrayOne to mergedArray
            {
                mergedArray[i] = arrayOne[locationOne];
                locationOne++;
                i++;
            }
        }
        else                                                            //If arrayOne is exhausted...
        {
            while(i < numItems*2)                                       //Add all remaining items from arrayTwo to mergedArray
            {
                mergedArray[i] = arrayTwo[locationTwo];
                locationTwo++;
                i++;
            }
        }
    }

    elapsedTime = clock() - elapsedTime;                                //End timer
    return ((float)(elapsedTime)/CLOCKS_PER_SEC);                       //Return duration (seconds)
}



void pthreadSort(int numItems, int* arrayOne, int* arrayTwo, int* mergedArray, double summary[2]) {
    int numThreads = 2;
    double mergeTime = 0;

    //CREATE THREAD STRUCTS {num items, array to be sorted, initial value of elapsedTime, array label}
    thread_args threadOneArgs = {numItems, arrayOne, 0, 1};
    thread_args threadTwoArgs = {numItems, arrayTwo, 0, 2};

    //ADD STRUCTS TO ARRAY
    thread_args threadArgs[numThreads];
    threadArgs[0] = threadOneArgs;
    threadArgs[1] = threadTwoArgs;

    //CREATE ARRAY TO HOLD THREADS
    pthread_t threads[numThreads];

    pthread_attr_t attr;                                                    //Declare a pthread attribute variable of the pthread_attr_t data type
    pthread_attr_init(&attr);                                               //Initialize the attribute variable with pthread_attr_init()
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);            //Set the attribute detached status with pthread_attr_setdetachstate()

    clock_t begin;
    begin = clock();                                                        //Start timer

    printf("\nClock started... %.5f\n", ((float)(clock()) - begin)/CLOCKS_PER_SEC);

    //INITIALIZE THREADS -> SORT ARRAYS
    int i;
    for(i = 0; i < numThreads; i++)
        pthread_create(&threads[i], &attr, threadSort, &threadArgs[i]);

    printf("\nThreads Created... %.5f\n", ((float)(clock()) - begin)/CLOCKS_PER_SEC);

    pthread_attr_destroy(&attr);                                            //Free library resources used by the attribute with pthread_attr_destroy()

    //JOIN THREADS
    for(i = 0; i < numThreads; i++)
        pthread_join(threads[i], NULL);

    printf("\nThreads Joined... %.5f\n", ((float)(clock()) - begin)/CLOCKS_PER_SEC);

    //AFTER THREADS HAVE JOINED,
    mergeTime = mergeArrays(numItems, arrayOne, arrayTwo, mergedArray);

    printf("\nArrays Merged... %.5f\n", ((float)(clock()) - begin)/CLOCKS_PER_SEC);

    //ADD TIMING DATA TO SUMMARY ARRAY
    summary[0] = ((float)(clock() - begin)/CLOCKS_PER_SEC);
    summary[1] = threadArgs[0].sortTime + threadArgs[1].sortTime;
    summary[2] = mergeTime;
}



void sortAndMerge(int numItems, int* arrayOne, int* arrayTwo, int* mergedArray, double summary[2]) {
    double sortTime = 0;
    double mergeTime = 0;

    printf("\nSORTING ARRAY ONE\n");
    sortTime = sortArray(numItems, arrayOne);
    printf("\nSORTING ARRAY TWO\n");
    sortTime += sortArray(numItems, arrayTwo);

    mergeTime = mergeArrays(numItems, arrayOne, arrayTwo, mergedArray);

    summary[0] = sortTime;
    summary[1] = mergeTime;
}



double sortDouble(int numItems, int* doubleArray) {
    printf("SORTING DOUBLE-LENGTH ARRAY");
    double elapsedTime = sortArray(numItems, doubleArray);
    return elapsedTime;
}






/*
 *EXPECTATIONS vs REALITY:
 *I originally expected the double-length array to take the longest to sort because
 *in the slower Big-O sorting methods, the n value was twice the size, which is a
 *significant increase. The results in that case were accurate, however, I was not
 *expecting that the concurrent and sequential sorting would take roughly the same
 *amount of time; I was certain that the concurrent sorting would take less than
 *half the amount of time of the sequential sorting. The overhead of creating and
 *merging threads could be so large that it mitigates the benefit of using threads
 *in the first place or the processor might be being utilized at 100% in the first
 *place, so splitting it up into two threads might reduce the performance of each
 *to 50% which would still result in the same overall time.
 */



