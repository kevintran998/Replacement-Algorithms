# Replacement-Algorithms

***** DESCRIPTION *****
This program tests four different page replacement algorithms to see which of the four is the most 
efficient in terms of the number of faults. The four algorithms include least-recently-used (LRU), 
first-in-first-out (FIFO), clock policy, and random replacement. It is important to know the efficiency 
of these four algorithms to know is best to implement in a virtual machine.

***** PROGRAM SPECIFICATIONS *****
- Generates random test data and inputed into the algorithms
- Outputs the average number of faults within 1000 different experiments

***** PROGRAMING LANGUAGE AND TOOLS *****
- Written in C++
- Xcode

***** WHAT I LEARNED *****
I learned how the four algorithms process data and create page faults. I also learned which algorithm
is the most efficient based on the number of page faults based on this experiment.

***** STEP BY STEP DESCRIPTION *****
The first step in creating this program is to perform a memory trace and generate 1000 random address 
integers for our test data. The test data must be consistent for all algorithms within a single experiment, 
so the program uses the same test data for all four algorithms before generating new integers. The test data 
will be passed through each algorithm in order: LRU, FIFO, clock, and random. One by one, the program will 
loop through each algorithm for working sets 2-20 and count the number of faults that occur in each algorithm 
based on the size of the working set. When all 1000 experiments are finished, the program will calculate the 
average number of faults and displays them for each algorithm and each working set size. After executing the 
program multiple times, I have concluded that FIFO and Clock are comparable to be the best, then LRU, then random. 
However, based on my experimental data, all four algorithms performed approximately the same.
