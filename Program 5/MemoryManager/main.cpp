#include <iostream>

#include "Process.h"
#include "MemoryManager.h"

const int M = 4;            //The minimum duration of the sampling interval (boundary protection for exceptional conditions)
const int L = 6;            //The maximum duration of the sampling interval (boundary protection for exceptional conditions)
const int Q = 3;            //The maximum # of page faults allowed between sampling instances
const int F = 16;           //The number of page frames
const int P = 4;            //The maximum # of processes that may be resident in main memory

using namespace std;

int main()
{
    MemoryManager VSWS;

    cout << "\n\n\nThank you for using Program 5.\n\n\n";
    exit(0);
}
