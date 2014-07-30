#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>

extern const int M, L, Q;

using namespace std;

class Process
{
public:
    Process(int PID, int size, ofstream& outFile);
    ~Process() { delete [] pageTable; }

    void performOperation(char operation, int page, stack<int>& freeFrames);
    void terminateProcess(stack<int>& freeFrames);

    int getPID() { return PID; }


private:
    typedef struct {
        char operation;
        int page;
    } instruction;

    typedef struct {
        bool workingSet;
        int frame;
    } allocation;

    int PID, pageTableSize, clock, numResident, numMemAccesses, currFaults, totalFaults;
    vector<int> preEvalResSetSize;
    allocation* pageTable;
    ofstream* outFile;

    queue<instruction> pendingInstructions;

    void read(int page);
    void write(int page);

    void evaluate(stack<int>& freeFrames);

    int getFreeFrame(stack<int>& freeFrames);

    string printPageTable();

    void logAndPrint(string output);

};

#endif // PROCESS_H
