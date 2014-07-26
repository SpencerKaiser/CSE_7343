#include "MemoryManager.h"
#include "Process.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

MemoryManager::MemoryManager()
{
    for(int i = F - 1; i >= 0; i--)
        freeFrames.push(i);                                     //Initialize stack of free frames (0, 1, 2, ... F-1, F)                             //Initialize stack of free frames (0, 1, 2, ... F-1, F)

    readInput();
}

void MemoryManager::readInput()
{
    system("pwd");
    ifstream systemTrace("VSWSsystemtrace.txt");
    if(!systemTrace)
    {
        cout << "Couldn't read System Trace text file." << endl;
        exit(0);
    }

    outFile.open("MemoryManagementLog.txt");

    char operation;
    int PID, sizeORpage;

    while(systemTrace.good())
    {
        systemTrace >> PID;
        systemTrace >> operation;
        systemTrace >> sizeORpage;

        switch(operation)
        {
        case 'n':
            createProcess(PID, sizeORpage);
            break;
        case 'r':
        case 'w':
            processOperation(PID, operation, sizeORpage);
            break;
        case 't':
            terminateProcess(PID);
            break;
        }
    }
    outFile.close();
    systemTrace.close();
}


void MemoryManager::createProcess(int PID, int size)
{
    if(processes.size() >= P)                               //Compare to MAX num of active processes
    {
        cout << "SAMPLE DATA EXCEEDS MAX NUMBER OF ALLOWABLE PROCESSES... EXITING." << endl;
        exit(1);
    }
    else
        processes.push_back(new Process(PID, size, outFile));
}


void MemoryManager::terminateProcess(int PID)
{
    for(unsigned long i = 0; i < processes.size(); i++)
    {
        if(processes.at(i)->getPID() == PID)
        {
            processes.at(i)->terminateProcess(freeFrames);
            processes.erase(processes.begin() + i);
            break;
        }
    }
}


void MemoryManager:: processOperation(int PID, char operation, int page)
{
    bool active = false;
    //FIND PROCESS & PERFORM OPERATION
    for(unsigned long i = 0; i < processes.size(); i++)             //Iterate through active processes
    {
        if(processes.at(i)->getPID() == PID)                        //If process is found...
        {
            processes.at(i)->performOperation(operation, page, freeFrames);     //Perform operation
            active = true;
            break;
        }
    }

    if(!active)                                                     //If process is inactive or has not yet been created...
    {
        cout << "The process being called could not be found... EXITING." << endl;
        exit(1);
    }
}
