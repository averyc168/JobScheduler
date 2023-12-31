# Job Scheduler

### Purpose: 
Takes an input file of jobs that contains the job id, arrival time, required time, and
required CPU. The program does calculations to see the start time of each job and the completion
time and then outputs the job ID and the completion time on the terminal, along with the
utilization rate of the jobs in either FIFO or FIFO+Backfill.

### Input: 
Job file that contains job id, arrival time, required time, and required CPU. Each job info is on a new line.

### Output: 
The program will output the job ID, and its corresponding completion time, and on the bottom
it has the system utilization rate for the scheduling process in either FIFO or FIFO+Backfill.

### To compile and run program, here is what you should do:
1. gcc main.c -o a.out
2. ./a.out *(number of CPUs) (input file) (schedule option (0 - FIFO or 1 - FIFO+Backfill))*


### Performance Analysis (all txt files are run with 4 CPUs):
jobs.txt
- FIFO: system utilization rate: 67.50%
- FIFO+Backfill: system utilization rate: 84.38%
jobs1.txt
- FIFO: system utilization rate: 100%
- FIFO+Backfill: system utilization rate: 100%
jobs2.txt
- FIFO: system utilization rate: 55%
- FIFO+Backfill: system utilization rate: 68.75%
jobsSmall.txt
- FIFO: system utilization rate: 56.82%
- FIFO+Backfill: system utilization rate: 62.50%
jobsLarge.txt
- FIFO: system utilization rate: 76.88%
- FIFO+Backfill: system utilization rate: 85.42%

Overall, the system utilization rate for the FIFO + backfill scheduling is a lot higher than FIFO, since in the
FIFO + backfill, it is using as many CPUs as possible, based on arrival times. With FIFO, each job is after its
previous and if the previous jobs do not take a lot of CPUs, then there will be many vacant CPUs.

jobs.txt
- FIFO
  - Avg Wait Time: 4 units of time
  - Avg Response Time: 4 units of time
  - Avg Turnaround Time: 6.4 units of time
- FIFO + Backfill
  - Avg Wait Time: 2.2 units of time
  - Avg Response Time: 2.2 units of time
  - Avg Turnaround Time: 4.2 units of time
jobs1.txt
- FIFO
  - Avg Wait Time: 0.6 units of time
  - Avg Response Time: 0.6 units of time
  - Avg Turnaround Time: 2.8 units of time
- FIFO + Backfill
  - Avg Wait Time: 0.6 units of time
  - Avg Response Time: 0.6 units of time
  - Avg Turnaround Time: 2.8 units of time
jobs2.txt
- FIFO
  - Avg Wait Time: 0.8 units of time
  - Avg Response Time: 0.8 units of time
  - Avg Turnaround Time: 1.6 units of time
- FIFO + Backfill
  - Avg Wait Time: 0.2 units of time
  - Avg Response Time: 0.2 units of time
  - Avg Turnaround Time: 1.2 units of time
jobsSmall.txt
- FIFO
  - Avg Wait Time: 2.6 units of time
  - Avg Response Time: 2.6 units of time
  - Avg Turnaround Time: 5 units of time
- FIFO + Backfill
  - Avg Wait Time: 1.2 units of time
  - Avg Response Time: 1.2 units of time
  - Avg Turnaround Time: 3.6 units of time
jobsLarge.txt
- FIFO
  - Avg Wait Time: 13.7 units of time
  - Avg Response Time: 13.7 units of time
  - Avg Turnaround Time: 16.2 units of time
- FIFO + Backfill
  - Avg Wait Time: 9.6 units of time
  - Avg Response Time: 9.6 units of time
  - Avg Turnaround Time: 12.1 units of time

Off of the average wait, response, and turnaround times between FIFO and FIFO+backfill for each of the text files,
you can tell that there is a decrease in all of the times and that the times are less for FIFO+backfill than FIFO.
This is because when jobs are being backfilled, they are finding CPUs freed up around their arrival time rather than
waiting for the previous job's start time. This cuts down the scheduling by a couple of units of time, which is more
efficient than just a FIFO scheduler and all of the CPUs are more likely to be in use.

### Extra (if you want to try):
- one sample input file of small job tests (5 jobs) --> jobsSmall.txt
- one sample input file of big job tests (20 jobs) --> jobsLarge.txt
