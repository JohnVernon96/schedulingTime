/*****************************************************************************
Name:		 John Carvajal
Class:		 CSC341
Date:		 April 26, 2018
File Name:	 341HW5.cpp
Description: You have been given the files 341HW5.cpp, OS_Threads.h and
input.txt. These files will provide a simulation of a single processor having
to schedule multiple different simultaneous threads. The main file creates the
object containing our simulated threads and runs all the scheduling functions.
You must modify the code in OS_Threds.h to implement the functions for running
the 3 following scheduling techniques:

Round Robin, Shortest Remaining Time Next, Highest Response Ratio Next.

You must print out the turn around time (TAT) for each thread and the normalized
TAT. Once all threads are complete you must print out the average (mean)
Normalized TAT for all the threads.

Input:  You have been provided an input file but it is advisable you make your
own to thoroughly test your program. The input file is structure like so:

First number is the number of threads to create
Each line afterwards contains 2 numbers separated by a single space
First number is the arrival time of the thread
Second number is the service time of the thread

You can always assume that each line will have an increasing arrival time
(meaning line 3 will never have an earlier arrival time than line 2)
******************************************************************************/

#include <iostream>
#include <fstream>
#include "OS_Threads.h"

using namespace std;

int main(int argc, char **argv)
{
	ifstream infile(argv[1]);
	int size;
	infile >> size;
	MultiThread t(size, 1);
	MultiThread tRR(size, 1);
	MultiThread tSRT(size, 1);
	MultiThread tHRR(size, 1);
	int num;
	int count = 0;
	while (infile >> num)
	{
		t.threads[count].threadID = count;
		tRR.threads[count].threadID = count;
		tSRT.threads[count].threadID = count;
		tHRR.threads[count].threadID = count;
		t.threads[count].timeCreated = num;
		tRR.threads[count].timeCreated = num;
		tSRT.threads[count].timeCreated = num;
		tHRR.threads[count].timeCreated = num;
		infile >> num;
		t.threads[count].timeService = num;
		tRR.threads[count].timeService = num;
		tSRT.threads[count].timeService = num;
		tHRR.threads[count].timeService = num;
		count++;
	}
	t.FCFS();
	tRR.RoundRobin();
	tSRT.ShortestRemainingTime();
	tHRR.HighestResponseRatio();
	return 0;
}
