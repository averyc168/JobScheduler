/*
 * Name: Avery Chan
 * Date: 11/7/23
 *
 * Program: Project 2
 *
 * Purpose: Takes an input file of jobs that contains the job id, arrival time, required time, and
 * required CPU. The program does calculations to see the start time of each job and the completion
 * time and then outputs the job id and the completion time on the terminal, along with the
 * utilization rate of the jobs in either FIFO or Backfill.
 *
 * Input: Job file that contains job id, arrival time, required time, and required CPU. Each job info
 * is on a new line.
 *
 * Output: The program will output the job id, and its corresponding completion time, and on the bottom
 * it has the system utilization rate for the scheduling process in either FIFO or Backfill.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct jobData {
    int	job_id;          // Job ID for each job
    int arrival_time;         // Arrival time of each job
    int req_time;        // Requested time for each job
    int req_cpus;        // Requested CPUs for each job
    int start_time;     // time of when the job started in the scheduler
    int completion_time;    // time when the job completed in the scheduler
};

/**
 * the function sees how many jobs there are in the file
 * @param infile - file pointer that holds the file contents
 * @param filename - name of the file
 * @return - returns the number of jobs in the file
 */
int getJobCount(FILE* infile, char* filename) {
    if ((infile = fopen(filename, "r")) == NULL) {  // checks to see if the file is open
        printf("FILE ERROR!");
        exit(1);
    }
    int length = 0; // number of jobs
    struct jobData jobStruct;
    while (!feof(infile)) {
        fscanf(infile, "%d%d%d%d", &jobStruct.job_id, &jobStruct.arrival_time, &jobStruct.req_time, &jobStruct.req_cpus);
        length++;
    }
    fclose(infile);
    return length;
}

/**
 * readFile takes in the filename and reads all the jobs and puts it into an array of jobData structs
 * @param fin - file pointer that holds the file contents
 * @param filename - filename that the user inputted
 * @param length - the number of jobs in the file
 * @return jobData* - the memory location of the array of jobData structs
 */
struct jobData* readFile(FILE* FIN, char* filename, int length) {
    if ((FIN = fopen(filename, "r")) == NULL) {  // checks to see if the file is open
        printf("FILE ERROR!");
        exit(1);
    }
    struct jobData *newArray;   // a pointer to the array of jobData structs
    newArray = malloc(length * sizeof(struct jobData)); // creating it with the size of number of jobs
    struct jobData jobStruct;
    int fileLength = 0; // iterator to make sure it is not greater than the number of jobs there are
    while (fileLength < length) {
        fileLength++;
        fscanf(FIN, "%d%d%d%d", &jobStruct.job_id, &jobStruct.arrival_time, &jobStruct.req_time, &jobStruct.req_cpus);
        newArray[fileLength-1] = jobStruct;
    }

    fclose(FIN);
    return newArray;
}

/**
 * fifoJobScheduler function takes all of the job data and calculates the start and complete times for all the jobs.
 * It also calculates the utilization rate after all of the jobs are put into the schedule.
 * @param jobArray - the array that contains all of the job info
 * @param length - the number of jobs in the file
 * @param CPUArray - an array that keeps track of the number of CPUs available at a certain timeframe
 * @param numOfCPUs  - the number of cpus the user entered
 * @return systemUtil - the percentage of cpus used when doing FIFO for the job schedule
 */
float FIFOJobScheduler(struct jobData jobArray[], int length, int CPUArray[], int numOfCPUs) {
    //printf("in function\n");
    int jobIter = 0;    // job iterator, makes sure it stays within the bounds of number of jobs
    int timeStamp;      // time iterator
    bool insert = false;    // boolean to see if the job can be completed and found a spot in the scheduler
    int consecutiveSeconds = 0; // number of consecutive seconds that there are free CPUs for a job

    while (jobIter < length) {
        if (jobIter == 0) {     // if it is the first job, the time iterator starts when the first job arrives
            timeStamp = jobArray[jobIter].arrival_time;
        }
        else {      // if not the first job
            // if the current job's arrival time is less than the previous job's start time
            if (jobArray[jobIter].arrival_time < jobArray[jobIter - 1].start_time) {
                timeStamp = jobArray[jobIter - 1].start_time;
            }
            else {
                timeStamp = jobArray[jobIter].arrival_time;
            }
        }
        insert = false;
        consecutiveSeconds = 0;
        while (!insert && timeStamp < 10000) {  // inner while loop to check when the job can be inserted
            if (CPUArray[timeStamp] >= jobArray[jobIter].req_cpus) {    // checks to see if min requirements are met for the job
                consecutiveSeconds++;
                if (consecutiveSeconds == jobArray[jobIter].req_time) { // checks to see if time matches the job requirements
                    insert = true;
                }
            }
            else {
                consecutiveSeconds = 0; // if streak breaks, then reset
            }
            timeStamp++;
        }
        if (insert) {
            // calculate start and completion times
            jobArray[jobIter].completion_time = timeStamp;
            jobArray[jobIter].start_time = timeStamp - consecutiveSeconds;
            // for loop to update number of free CPUs
            for (int i = timeStamp - consecutiveSeconds; i < timeStamp; i++) {
                CPUArray[i] -= jobArray[jobIter].req_cpus;
            }
            jobIter++;
        }
    } // endWhile

    for (int i = 0; i < length; i++) {
        printf("%d %d\n", jobArray[i].job_id, jobArray[i].completion_time);
    }

    int maxTime = jobArray[0].completion_time;  // the starting max time
    // loop to see how long it took to complete all the jobs in the file
    for (int j = 0; j < length; j++) {
        if (jobArray[j].completion_time >= maxTime) {
            maxTime = jobArray[j].completion_time;
        }
    } // endFor

    int totalCPUs = numOfCPUs * maxTime;    // gets number of total CPUs
    int unusedCPUs = 0; // gets number of unused CPUs through the loop below
    for (int k = 0; k < maxTime; k++) {
        unusedCPUs += CPUArray[k];
    }
    float sysUtil = ((float)(totalCPUs - unusedCPUs) / totalCPUs * 100); // calculates the total system utilization rate
    return sysUtil;
}

/**
 * this function schedules the job through backfill method, which should be more optimized than fifo
 * @param jobArray - array of job data
 * @param length - the number of jobs in file
 * @param CPUArray - an array to keep track of which CPUs are free in a 2D array
 * @param numOfCPUs - number of CPUs used
 * @return - returns a float that calculates system utilization rate for backfill scheduling
 */
float BackFillJobScheduler(struct jobData jobArray[], int length, int CPUArray[][10000], int numOfCPUs) {
    int jobIter = 0;    // job iterator
    int timeStamp;      // time iterator
    bool insert = false;    // checks to see if the job can be inserted into schedule
    int consecutiveSeconds = 0; // number of seconds that there are enough free CPUs
    int CPUIter = 0; // CPU iterator for the 2D array
    int CPUCounter = 0; // counts the number of free CPUs within a timeframe

    while (jobIter < length) {  // while within the range of number of jobs
        insert = false;
        consecutiveSeconds = 0;
        CPUIter = 0;
        timeStamp = jobArray[jobIter].arrival_time; // makes the time iterator start at the job's arrival time
        while (!insert && timeStamp < 10000) {  // while seeing where the job is going to fit
            if (consecutiveSeconds == 0) {  // resets the CPU iterator if the streak is broken
                CPUIter = 0;
            }
            bool done = false;  // another boolean value to help with the innermost while loop
            // while loop that iterates through all the CPUs to see which are free and how many of them are free
            while (!done && !insert && CPUIter < numOfCPUs) {
                CPUCounter = 0;
                if (CPUArray[CPUIter][timeStamp] == 0) {    // if CPU is free
                    // loop to count how many CPUs are free at this timeframe
                    for (int i = CPUIter; i < numOfCPUs; i++) {
                        if (CPUArray[i][timeStamp] == 0) {
                            CPUCounter++;
                        }
                    }
                    // if number of CPU's fills the minimum requirement of job
                    if (CPUCounter >= jobArray[jobIter].req_cpus) {
                        consecutiveSeconds++;
                        if (consecutiveSeconds == jobArray[jobIter].req_time) {
                            insert = true;
                        }
                    }
                    else {
                        consecutiveSeconds = 0; // break streak
                    }
                    done = true;
                }
                else {
                    CPUIter++;
                    // if the iterator is at 4, have to reset it
                    if (CPUIter == 4) {
                        consecutiveSeconds = 0;
                    }
                }
            } // endInnerWhile (sees how many CPUs there are)
            timeStamp++;
        } // endSecondWhile (trying to find a spot for the job)

        if (insert) {
            jobArray[jobIter].completion_time = timeStamp;
            jobArray[jobIter].start_time = timeStamp - consecutiveSeconds;
            // loop to update which CPUs are taken and occupied at certain timeframes
            for (int i = CPUIter; i < CPUIter + jobArray[jobIter].req_cpus; i++) {
                for (int j = timeStamp - consecutiveSeconds; j < timeStamp; j++) {
                    CPUArray[i][j] = 1;
                }
            }
            jobIter++;
        }
    } // endMainWhile

    for (int i = 0; i < length; i++) {
        printf("%d %d\n", jobArray[i].job_id, jobArray[i].completion_time);
    }

    int maxTime = jobArray[0].completion_time; // set the first completion time to when the whole scheduling is done
    // loop to see when the actual completion time for the scheduling is done
    for (int j = 0; j < length; j++) {
        if (jobArray[j].completion_time >= maxTime) {
            maxTime = jobArray[j].completion_time;
        }
    } // endFor

    int totalCPUs = numOfCPUs * maxTime;    // number of CPUs used during the scheduling process
    int unusedCPUs = 0; // number of unused CPUs during the scheduling process, which is found through loop below
    for (int k = 0; k < numOfCPUs; k++) {
        for (int l = 0; l < maxTime; l++) {
            if (CPUArray[k][l] == 0) {
                unusedCPUs++;
            }
        }
    }
    float sysUtil = (((float)((totalCPUs - unusedCPUs)) / totalCPUs) * 100); // calculates the system utilization rate
    return sysUtil;
}

int main(int argc, char* argv[]) {
    FILE *infile, *FIN; // two file pointers for the one file
    int numOfCPUs = atoi(argv[1]);  // number of CPUs inputted by user
    char* filename = argv[2]; // filename inputted by user
    int option = atoi(argv[3]); // option 0 for FIFO, 1 for backfill by user
    struct jobData *jobArray;   // pointer to an array of jobData structs

    if ((infile = fopen(filename, "r")) == NULL) {  // checks to see if the file is open
        printf("FILE ERROR!");
        exit(1);
    }
    int length = getJobCount(infile, filename); // gets the number of jobs in the file

    if ((FIN = fopen(filename, "r")) == NULL) {  // checks to see if the file is open
        printf("FILE ERROR!");
        exit(1);
    }
    jobArray = readFile(FIN, filename, length); // sets the address to the new array of jobData structs

    if (option == 0) {  // if FIFO
        int CPUArray[10000];
        for (int i = 0; i < 10000; i++) {   // sets the CPU array for number of CPU's available
            CPUArray[i] = numOfCPUs;
        }

        // calculate system utilization rate for FIFO scheduling
        float sysUtil = FIFOJobScheduler(jobArray, length, CPUArray, numOfCPUs);
        printf("System Utilization Rate is %.2f%%", sysUtil);
    }
    else if (option == 1) { // if Backfill
        int CPUArray[numOfCPUs][10000];
        for (int i = 0; i < numOfCPUs; i++) {   // sets 2D array for which CPUs are available
            for (int j = 0; j < 10000; j++) {
                CPUArray[i][j] = 0;
            }
        }

        // calculate system utilization rate for FIFO scheduling
        float sysUtil = BackFillJobScheduler(jobArray, length, CPUArray, numOfCPUs);
        printf("System Utilization Rate is %.2f%%", sysUtil);
    }
    else {  // if neither option is selected then end the code
        printf("Not a valid option! Run it back.\n");
        exit(-2);
    }

    free(jobArray);
    jobArray = NULL;

    return 0;
}
