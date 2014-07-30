#include <iostream>
#include <sstream>
#include <stack>
#include <iomanip>
#include <sstream>

#include "Process.h"

using namespace std;

Process::Process(int PID, int size, ofstream& outFile) : PID(PID), pageTableSize(size), clock(0), numResident(0), numMemAccesses(0), currFaults(0), totalFaults(0), outFile(&outFile)
{
    pageTable = new allocation[pageTableSize];

    //Initialize all page frame values to -1 & resident status to FALSE
    for(int i = 0; i < pageTableSize; i++)
    {
        pageTable[i].frame = -1;
        pageTable[i].workingSet = false;
    }

    stringstream output;
    output << "Process " << PID << " is created.  Page table size is " << pageTableSize << ".  Number of resident pages is " << numResident << "." << endl;
    logAndPrint(output.str());
}

void Process::performOperation(char operation, int page, stack<int>& freeFrames)
{
    if(pageTable[page].frame < 0 && freeFrames.empty())                                     //Page is NOT in resident set and additional memory does NOT exist
    {
        stringstream output;
        output << "ERROR: FREE FRAME NOT FOUND (Process " << PID << ", Page " << page << ")" << endl;
        logAndPrint(output.str());

        pageTable[page].frame = -2;
    }
    else
    {
        if(pageTable[page].frame < 0)                                                           //If page isn't in resident set, allocate a frame for it
        {
            pageTable[page].frame = getFreeFrame(freeFrames);
            ++numResident;
            ++currFaults;
        }

        pageTable[page].workingSet = true;

        if(operation == 'r')
            read(page);
        else if (operation == 'w')
            write(page);
        else
        {
            string output = "INVALID OPERATION... EXITING\n";
            logAndPrint(output);
            exit(1);
        }

        ++clock;
        ++numMemAccesses;
    }                                                                                       //PAGE IS NOT IN RESIDENT SET && MEMORY IS UNAVAILABLE


    if(clock == L || (clock >= M && currFaults >= Q))
    {
        stringstream output;
        output << "\nEVALUATING WORKING SET FOR PROCESS " << PID << " (Clock = " << clock << ", NumFaults = " << currFaults << ")" << endl;
        logAndPrint(output.str());
        evaluate(freeFrames);
    }
}

void Process::read(int page)
{
    stringstream output;
    output << "Process " << PID << " is reading from virtual page " << page << " which occupies frame " << pageTable[page].frame << ".  Number of resident pages is " << numResident << " " << printPageTable() << "." << endl;
    logAndPrint(output.str());
}

void Process::write(int page)
{
    stringstream output;
    output << "Process " << PID << " is writing to virtual page " << page << " which occupies frame " << pageTable[page].frame << ".  Number of resident pages is " << numResident << " " << printPageTable() << "." << endl;
    logAndPrint(output.str());
}

void Process::evaluate(stack<int>& freeFrames)
{
    totalFaults += currFaults;
    preEvalResSetSize.push_back(numResident);

    stringstream output;
    output << "\tBEGIN EVALUATION...\n";

    for(int i = 0; i < pageTableSize; i++)
    {
        if(pageTable[i].workingSet == false && pageTable[i].frame >= 0)         //ALLOCATED & NOT PART OF WORKING SET -> REMOVE
        {
            output << "\t\tRemoving pageTable[" << i << "] (" << pageTable[i].frame << ") from resident set because workingSet == " << pageTable[i].workingSet << endl;
            freeFrames.push(pageTable[i].frame);
            pageTable[i].frame = -1;
            --numResident;
        }
        else if(pageTable[i].frame >= 0)                                        //ALLOCATED & PART OF WORKING SET -> MARK & TEST LATER
        {
            output << "\t\tMarking pageTable[" << i << "] (" << pageTable[i].frame << ") as NOT being part of working set..." << endl;
            pageTable[i].workingSet = false;
        }
    }
    output << "\tEND EVALUATION...\n";

    logAndPrint(output.str());

    clock = currFaults = 0;

    output.clear();
    output.str(string());
    output << "Process " << PID << " use-bits scanned.  New resident set size is " << numResident << ".   Resident set: " << printPageTable() << ".\n\n";
    logAndPrint(output.str());
}

string Process::printPageTable()
{
    int numMatches = 0;
    stringstream pageMatches;
    pageMatches << "(";
    for(int i = 0; i < pageTableSize; i++)
    {
        if(pageTable[i].frame >= 0)
        {
            if(numMatches > 0)
                pageMatches << " , ";
            pageMatches << i << ":" << pageTable[i].frame;
            numMatches++;
        }
    }
    pageMatches << ")";
    return pageMatches.str();
}

int Process::getFreeFrame(stack<int>& freeFrames)
{
    if(!freeFrames.empty())
    {
        int freeFrame = freeFrames.top();
        freeFrames.pop();
        return freeFrame;
    }
    else
        return -2;
}

void Process::terminateProcess(stack<int>& freeFrames)
{
    for(int i = 0; i < pageTableSize; i++)
    {
        if(pageTable[i].frame >= 0)
            freeFrames.push(pageTable[i].frame);
    }

    int sumResident = 0;

    for(unsigned long i = 0; i < preEvalResSetSize.size(); i++)
        sumResident += preEvalResSetSize.at(i);

    float avgResidentSize = (float)sumResident/(float)preEvalResSetSize.size();


    stringstream output;
    output << fixed << setprecision(3);
    output << "Process " << PID << " terminated.  Total memory accesses: " << numMemAccesses << ".   Total page faults: " << totalFaults << ". Page fault percentage: " << (float)totalFaults / (float)numMemAccesses << ".   Average resident set size: " << avgResidentSize << "." << endl;
    logAndPrint(output.str());
}

void Process::logAndPrint(string output)
{
    cout << output;
    *outFile << output;
}
