/********************************************
*** NAME         : Kyle Paxton            ***
*** CLASS        : CSC 456                ***
*** ASSIGNMENT   : Assignment 3           ***
*** DUE DATE     : 11/30/17               ***
*** INSTRUCTOR   : Myounggyu Won          ***
********************************************/

#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
using namespace std;

void memoryManager(int, int);
int allocate(int, int);
int deallocate(int);
int write(int, int);
int read(int, int);
void printMemory();
void splitString(string[], string);
void setMethod(string);

const int MAX_SIZE = 100;

struct process
{
	int pid;
	int totalFrames;
	int pageTable[MAX_SIZE];
};

unsigned char phyMem[MAX_SIZE]; //each element is a frame
bool freeFrameList[MAX_SIZE];
bool fifo = false;
bool lru = false;
string arguments[MAX_SIZE];
vector<process> processes;
int totalFrames = 0;
int processCount = 0;

int main()
{

	bool continueWhile = true;
	while (continueWhile)
	{
		string command = "";
		getline(cin, command);
		splitString(arguments, command);
		if (arguments[0] == "M")
		{
			memoryManager(atoi(arguments[1].c_str()), atoi(arguments[2].c_str()));
		}
		else if (arguments[0] == "A")
		{
			allocate(atoi(arguments[1].c_str()), atoi(arguments[2].c_str()));
		}
		else if (arguments[0] == "D")
		{
			deallocate(atoi(arguments[1].c_str()));
		}
		else if (arguments[0] == "W")
		{
			write(atoi(arguments[1].c_str()), atoi(arguments[2].c_str()));
		}
		else if (arguments[0] == "R")
		{
			read(atoi(arguments[1].c_str()), atoi(arguments[2].c_str()));
		}
		else if (arguments[0] == "P")
		{
			printMemory();
		}
		else if (arguments[0] == "X")
		{
			continueWhile = false;//exit
		}
		else if (arguments[0] == "SET" || arguments[0] == "set" || arguments[0] == "Set")
		{
			setMethod(arguments[1]);//exit
		}
		else
		{
			cout << "Invalid command! Please enter 'M', 'W', 'A', 'R', 'D', 'P', 'X' or 'SET'" << endl;
		}

	}

}

void setMethod(string algorithm)
{
	if (algorithm == "Fifo" || algorithm == "fifo" || algorithm == "FIFO")
	{
		fifo = true;
		lru = false;
		cout << "Page replacement: FIFO" << endl << endl;

	}
	else if (algorithm == "Lru" || algorithm == "lru" || algorithm == "LRU")
	{
		fifo = false;
		lru = true;
		cout << "Page replacement: LRU" << endl << endl;
	}
	else
	{
		cout << "Page replacement not set! Please use 'LRU' or 'FIFO'" << endl << endl;
	}

}

void splitString(string arguments[], string command)
{
	string search = " ";
	int spacePos;
	int currPos = 0;
	int k = 0;
	int prevPos = 0;

	do
	{

		spacePos = command.find(search, currPos);

		if (spacePos >= 0)
		{

			currPos = spacePos;
			arguments[k] = command.substr(prevPos, currPos - prevPos);
			currPos++;
			prevPos = currPos;
			k++;
		}


	} while (spacePos >= 0);

	arguments[k] = command.substr(prevPos, command.length());
}

//Creates physical memory space
void memoryManager(int memSize, int frameSize)
{
	for (int i = 0; i< memSize; i++)
	{
		phyMem[i] = '0';
		freeFrameList[i] = true;// all free frames will be available to start
	}
	totalFrames = memSize;
}

//Uses pid to allocate pages to a process
int allocate(int allocSize, int pid)
{
	process newProcess;
	newProcess.pid = pid;
	newProcess.totalFrames = allocSize;
	int allocatedFrames = 0;
	int i = 0;
	int x = 0;

	if (allocSize < totalFrames)
	{
		while (allocatedFrames < allocSize)
		{
			i = rand() % totalFrames;
			if (freeFrameList[i] == true)
			{
				newProcess.pageTable[x] = i;
				freeFrameList[i] = false;
				allocatedFrames++;
				x++;
			}
		}
		processes.push_back(newProcess);
		return 1;
	}
	else
	{
		cout << "Not enough memory to allocate!" << endl;
		return -1;
	}
}

//deallocates memory from a process
int deallocate(int pid)
{
	bool found = false;
	for (int i = 0; i < processes.size(); i++)
	{
		if (processes[i].pid == pid)
		{
			found = true;
			for (int j = 0; j < processes[i].totalFrames; j++) {
				freeFrameList[processes[i].pageTable[j]] = true;
			}
			processes.erase(processes.begin() + i);

			return 1;
		}
	}
	if (found == false)
	{
		return -1;
	}
}

// writes "1" to the address specified to a process with the specified pid
int write(int pid, int logical_address)
{
	int i = 0;
	bool found = false;
	while (i < processes.size() && found == false)
	{
		if (processes[i].pid = pid)
		{
			found = true;
			if (logical_address <= processes[i].totalFrames)
			{
				phyMem[processes[i].pageTable[logical_address]] = '1';
			}
			else {
				return -1;
			}
		}
		else {
			i++;
		}
	}
	return 1;
}

//returns the memory space value of the process with the specified pid.
int read(int pid, int logical_address)
{
	int i = 0;
	while (i < processes.size())
	{
		if (processes[i].pid = pid)
		{
			if (logical_address < processes[i].totalFrames)
			{
				cout << "Memory space of process " << processes[i].pid << ": " << phyMem[processes[i].pageTable[logical_address]] << endl;
				return phyMem[processes[i].pageTable[logical_address]];
			}
			else {
				return -1;
			}
		}
		else {
			i++;
		}
	}
	return -1;
}

// Prints out the list of physical memory space, the free frame list, and the process list. 
void printMemory()
{
	cout << endl;
	cout << "Physical Memory Space:" << endl;
	for (int i = 0; i < totalFrames; i++)
	{
		cout << phyMem[i] << " ";
	}

	cout << endl << endl;
	cout << "Free Frame List:" << endl;
	for (int i = 0; i < totalFrames; i++)
	{
		if (freeFrameList[i] == true)
		{
			cout << i << " ";
		}
	}

	cout << endl << endl;
	cout << "Process List:" << endl;
	for (int i = 0; i < processes.size(); i++)
	{
		cout << processes[i].pid << " " << processes[i].totalFrames << endl;

	}
	cout << endl;
}

