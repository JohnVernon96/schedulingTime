/*****************************************************************************
Name:		 John Carvajal
Class:		 CSC341
Date:		 April 26, 2018
File Name:	 OS_Threads.h
Description: This is the header file used with 341HW5.cpp
******************************************************************************/
#ifndef OS_THREADS_H
#define OS_THREADS_H
#include <iostream>
#define  LARGE_INT_VAL 65535

//Stores some basic information to simulate a thread running an application
class MyThread
{
public:
    MyThread()
    {
        threadID = -1;
        timeCreated = -1;
        timeService = -1;
        turnAroundTime = -1;
    }
    int threadID;
    int timeCreated;
    int timeService;
    int turnAroundTime;
};

//Creates an array of threads
class MultiThread
{
public:
    //creates the array for the threads.
    MultiThread(int threadNum, int quant)
    {
        threads = new MyThread[threadNum];
        threadNumber = threadNum;
        timeQuantum = quant;
        totalTAT = 0;
    }
    ~MultiThread()
    {
        delete []threads;
    }

    //initializes all of the values inside of each thread
    //in the array.
    int Thread_Init(int,int,int);

    //Implements first come first serve
    void FCFS();

    //prints each thread info. Takes the thread ID as an argument
    void printThread(int);

    void RoundRobin();

    void ShortestRemainingTime();

    void HighestResponseRatio();

    MyThread *threads;
private:
    //Simulates performing action for a time quantum
    void wait();

    //Prints the mean TAT.
    void printMean(double);

    //Prints the time.
    void printTime(int);

    //Displays which thread is getting to run.
    void doWork(int);

    //Keeps track of size of array of threads.
    int threadNumber;

    //Stores the time quantum.
    int timeQuantum;

    double totalTAT;
};

int MultiThread::Thread_Init(int id, int create, int need)
{
    if (id >= 0 && id < threadNumber)
    {
        threads[id].threadID = id;
        threads[id].timeCreated = create;
        threads[id].timeService = need;
        return 0;
    }
    else
        return -1;
}

void MultiThread::wait()
{
    for (int i = 0; i < 100000000; i++);
}

void MultiThread::printMean( double aver )
{
    std::cout << "Average (mean) of normalized TAT: " << totalTAT / threadNumber << std::endl;
}

void MultiThread::printThread(int id)
{
    totalTAT += (double)threads[id].turnAroundTime / (double)threads[id].timeService;
    std::cout << "Thread T[" << id << "]: Turn around time " << threads[id].turnAroundTime
        << ", and Normalized TAT " << (double)threads[id].turnAroundTime / (double)threads[id].timeService << std::endl;
}
void MultiThread::printTime(int tm)
{
//std::cout << "Time is: " << tm << "  "  << std::endl;
}

void MultiThread::doWork(int id)
{
   std::cout << "Thread T[" << id << "] started and running." << std::endl;
}
void MultiThread::FCFS()
{
    std::cout << std::endl;
    std::cout << "FIRST COME FIRST SERVED HAS BEEN CALLED" << std::endl;

    int time = 0;
    double average = 0;
    for (int i = 0; i < threadNumber; i++)
    {
        doWork(i);
        for (int j = 0; j < threads[i].timeService; j++)
        {
            time++;
            wait();
        }
        threads[i].turnAroundTime = time - threads[i].timeCreated;
        //average TAT = total TAT / number of threads
        average += (double)threads[i].turnAroundTime;
        //normalized turn around time (TAT) = turn around time / service time
        printThread(i);
    } //end for
    //print the average (mean) of normalized TAT
    printMean( average );
}

void MultiThread::RoundRobin()
{
    std::cout << std::endl;
    std::cout << "ROUND ROBIN HAS BEEN CALLED" << std::endl;

    //main time loop
    int totalTime = 0;

    //get the total time starting at the max
    totalTime = threads[threadNumber-1].timeCreated;
    for (int i = 0; i < threadNumber; i++)
    {
        threads[i].turnAroundTime = 0;
        totalTime += threads[i].timeService;
    }
    //std::cout << "Max Total time is: " << totalTime << std::endl;

    //use count down timer to keep track of working threads
    int numThreadFound = threadNumber;
    int i = 0;
    int time = 0;
    int numThreadsRemaining = 0;
    double average = 0;
    printTime( time );
    while( time <= totalTime )
    {

        if (time >= threads[i].timeCreated && threads[i].timeService > threads[i].turnAroundTime)
        {
            //thread is ready to run
            //do work
            doWork(i);
            wait();
            threads[i].turnAroundTime += timeQuantum;

            //increment time
            time += timeQuantum;
            printTime( time );

            //reset the index counter
            numThreadFound = threadNumber;

            //print when thread finished
            if (  threads[i].turnAroundTime >= threads[i].timeService )
            {
                numThreadsRemaining += 1;
                //set turnAroundTime to true time
                threads[i].turnAroundTime = time - threads[i].timeCreated;
                // average TAT = total TAT / number of threads
                average += (double)threads[i].turnAroundTime;
                // normalized turn around time (TAT) = turn around time / service time
                printThread(i);
            }
        }
         else
        {
            //thread is not ready to run
            numThreadFound--;
        }

        // check for next time increment if no thread was read
        if(numThreadFound < 1)
        {
            //increment time
            time += timeQuantum;
            printTime(time);
            //reset the counter
            numThreadFound = threadNumber;
        }

        // check the new index thread value
        if ((i % threadNumber) == threadNumber-1)
        {
            //reset to the first thread
            i = 0;
        }
        else
        {
            //set to next thread
            i++;
        }

        //check of all process are completed
        if(numThreadsRemaining >= threadNumber)
        {
            //done with threads, exit whilte loop
            break;
        }

    } //end wile

    //print the average (mean) of normalized TAT
    printMean(average);
}

void MultiThread::ShortestRemainingTime()
{
    std::cout << std::endl;
    std::cout << "SHORTEST REMAINING TIME HAS BEEN CALLED" << std::endl;

    int remainingTime[threadNumber];
    int totalTime = 0;
    //get the total time starting at the max
    totalTime = threads[threadNumber-1].timeCreated;
    for (int i = 0; i < threadNumber; i++)
    {
        threads[i].turnAroundTime = 0;
        remainingTime[i] = threads[i].timeService;
        totalTime += threads[i].timeService;
    }
    //std::cout << "Max Total time is: " << totalTime << std::endl;

    int time = 0;
    double average = 0;
    while(time <= totalTime)
    {
        printTime(time);

        //find the shortest remaining time, starting at the first one
        int iSRT = -1;
        int SRTValue =  LARGE_INT_VAL;
        for (int i = 0; i < threadNumber; i++)
        {
            if (time >= threads[i].timeCreated && remainingTime[i] < SRTValue)
            //thread is ready to run
            {
                SRTValue = threads[i].timeService;
                iSRT = i;
            }
        }

        //check of all process are completed
        if (iSRT < 0)
        {
            // all processed
            break;
        }
        else
        {
            //do work on thread iSRT
            doWork(iSRT);
            wait();
            remainingTime[iSRT] = remainingTime[iSRT] - timeQuantum;

            //print when thread finished
            if (remainingTime[iSRT] <= 0)
            {
                //set turnAroundTime to correct value
                threads[iSRT].turnAroundTime = time + timeQuantum - threads[iSRT].timeCreated;
                //average TAT = total TAT / number of threads
                average += (double)threads[iSRT].turnAroundTime;
                //normalized turn around time (TAT) = turn around time / service time
                printThread(iSRT);
                remainingTime[iSRT] = LARGE_INT_VAL;
            }
            //increment time
            time += timeQuantum;
        }
    } //end while
    //print the average (mean) of normalized TAT
    printMean(average);
}

void MultiThread::HighestResponseRatio()
{
    std::cout << std::endl;
    std::cout << "HIGHEST RESPONSE RATIO HAS BEEN CALLED" << std::endl;

    int remainingTime[threadNumber];
    int waitingTime[threadNumber];
    int responseRatio[threadNumber];
    int totalTime = 0;
    //get the total time starting at the max
    totalTime = threads[threadNumber-1].timeCreated;
    for (int i = 0; i < threadNumber; i++)
    {
        threads[i].turnAroundTime = 0;
        remainingTime[i] = threads[i].timeService;
        waitingTime[i] =0;
        totalTime += threads[i].timeService;
    }
    //std::cout << "Max Total time is: " << totalTime << std::endl;

    int time = 0;
    double average = 0;
    while(time <= totalTime)
    {
        printTime(time);

        // find the appropriate process to be scheduled
        int iHRR = -1;
        int HRRValue =  0;
        // look for thread with highest response ratio
        for (int  i = 0; i < threadNumber; i++)
        {
            //waiting time =  time - time created - time service;
            waitingTime[i] = time + timeQuantum - threads[i].timeCreated; // -  remainingTime[i];
            //response ratios  = (wait time + service time) / service time
            responseRatio[i] = (waitingTime[i] + threads[i].timeService) / (threads[i].timeService);

            //find the thread with Highest Response Ratio
            if (time >= threads[i].timeCreated && responseRatio[i] > HRRValue  && remainingTime[i] > 0)
            //thread is ready to run
            {
                HRRValue = responseRatio[i];
                iHRR = i;
            }
        }
        //check of all process are completed
        if (iHRR < 0)
        {
            // all processed
            break;
        }
        else
        {
            //do work on thread iHRR
            doWork(iHRR);
            wait();
            remainingTime[iHRR] = remainingTime[iHRR] - timeQuantum;

            //print when thread finished
            if (remainingTime[iHRR] <= 0)
            {
                threads[iHRR].turnAroundTime = time + timeQuantum - threads[iHRR].timeCreated;
                //average TAT = total TAT / number of threads
                average += (double)threads[iHRR].turnAroundTime;
                //normalized turn around time (TAT) = turn around time / service time
                printThread(iHRR);
                remainingTime[iHRR] = 0;
            }
            //increment time
            time += timeQuantum;
        }
    } //end while
    //print the average (mean) of normalized TAT
    printMean(average);
}
#endif
