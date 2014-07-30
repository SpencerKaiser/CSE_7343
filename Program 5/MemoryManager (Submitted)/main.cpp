#include <iostream>

#include "Process.h"
#include "MemoryManager.h"

const int M = 4;            //The minimum duration of the sampling interval (boundary protection for exceptional conditions)
const int L = 6;            //The maximum duration of the sampling interval (boundary protection for exceptional conditions)
const int Q = 3;            //The maximum # of page faults allowed between sampling instances
const int F = 32;           //The number of page frames
const int P = 4;            //The maximum # of processes that may be resident in main memory

using namespace std;

int main()
{
    cout << "\n\n\nWELCOME TO PROGRAM 5\n\n\n";

    MemoryManager VSWS;

    cout << "\n\n\nThank you for using Program 5.\n\n\n";
    exit(0);
}


/*INITIAL VALUES (4, 6, 3)
 * Process 0 terminated.  Total memory accesses: 21.   Total page faults: 12. Page fault percentage: 0.571.   Average resident set size: 5.750.
 * Process 1 terminated.  Total memory accesses: 15.   Total page faults: 5. Page fault percentage: 0.333.   Average resident set size: 4.000.
 * Process 2 terminated.  Total memory accesses: 32.   Total page faults: 12. Page fault percentage: 0.375.   Average resident set size: 5.167.
 * Process 3 terminated.  Total memory accesses: 19.   Total page faults: 9. Page fault percentage: 0.474.   Average resident set size: 5.333.
 *
 * FIRST ATTEMPT (2, 6, 2)
 * Process 0 terminated.  Total memory accesses: 21.   Total page faults: 14. Page fault percentage: 0.667.   Average resident set size: 4.143.
 * Process 1 terminated.  Total memory accesses: 15.   Total page faults: 10. Page fault percentage: 0.667.   Average resident set size: 3.800.
 * Process 2 terminated.  Total memory accesses: 32.   Total page faults: 14. Page fault percentage: 0.438.   Average resident set size: 4.000.
 * Process 3 terminated.  Total memory accesses: 19.   Total page faults: 8. Page fault percentage: 0.421.   Average resident set size: 4.250.
 *
 * SECOND ATTEMPT (6, 8, 4)
 * Process 0 terminated.  Total memory accesses: 21.   Total page faults: 8. Page fault percentage: 0.381.   Average resident set size: 6.000.
 * Process 1 terminated.  Total memory accesses: 15.   Total page faults: 6. Page fault percentage: 0.400.   Average resident set size: 5.000.
 * Process 2 terminated.  Total memory accesses: 32.   Total page faults: 11. Page fault percentage: 0.344.   Average resident set size: 5.750.
 * Process 3 terminated.  Total memory accesses: 19.   Total page faults: 8. Page fault percentage: 0.421.   Average resident set size: 6.000.
 *
 * THIRD ATTEMPT (8, 10, 8)
 * Process 0 terminated.  Total memory accesses: 21.   Total page faults: 9. Page fault percentage: 0.429.   Average resident set size: 7.500.
 * Process 1 terminated.  Total memory accesses: 15.   Total page faults: 5. Page fault percentage: 0.333.   Average resident set size: 5.000.
 * Process 2 terminated.  Total memory accesses: 32.   Total page faults: 8. Page fault percentage: 0.250.   Average resident set size: 6.333.
 * Process 3 terminated.  Total memory accesses: 19.   Total page faults: 5. Page fault percentage: 0.263.   Average resident set size: 5.000.
 *
 * FOURTH ATTEMPT (1, 8, 1)
 * Process 0 terminated.  Total memory accesses: 21.   Total page faults: 17. Page fault percentage: 0.810.   Average resident set size: 2.118.
 * Process 1 terminated.  Total memory accesses: 15.   Total page faults: 10. Page fault percentage: 0.667.   Average resident set size: 2.200.
 * Process 2 terminated.  Total memory accesses: 32.   Total page faults: 26. Page fault percentage: 0.812.   Average resident set size: 2.154.
 * Process 3 terminated.  Total memory accesses: 19.   Total page faults: 12. Page fault percentage: 0.632.   Average resident set size: 2.500.
 *
 * FIFTH ATTEMPT (1, 8, 2)
 * Process 0 terminated.  Total memory accesses: 21.   Total page faults: 14. Page fault percentage: 0.667.   Average resident set size: 4.143.
 * Process 1 terminated.  Total memory accesses: 15.   Total page faults: 10. Page fault percentage: 0.667.   Average resident set size: 3.800.
 * Process 2 terminated.  Total memory accesses: 32.   Total page faults: 14. Page fault percentage: 0.438.   Average resident set size: 4.429.
 * Process 3 terminated.  Total memory accesses: 19.   Total page faults: 8. Page fault percentage: 0.421.   Average resident set size: 4.250.
 *
 *
 * The third attempt was the most successful by lowering the fault percentage as it waited the longest between sampling intervals, resulting in
 * large working set sizes. Because a large portion of the program was in frames, there were relitively few page faults.
 *
 */
