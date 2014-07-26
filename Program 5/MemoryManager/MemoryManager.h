#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "Process.h"
#include <vector>
#include <stack>
#include <fstream>

extern const int F, P;

using namespace std;

class MemoryManager
{
public:
    MemoryManager();
    void readInput();
    void createProcess(int PID, int size);
    void terminateProcess(int PID);
    void processOperation(int PID, char operation, int page);

private:
    vector<Process*> processes;
    stack<int> freeFrames;
    ofstream outFile;

};

#endif // MEMORYMANAGER_H
