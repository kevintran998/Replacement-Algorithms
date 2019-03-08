// File: main.cpp
// Author: Kevin Tran
//
// main: generates memory traces, performs function calls for each algorithm, keeps a running total on the number of faults for each algorithm, then displays the average number of faults onto the screen
// LRU: code for least-recently-used algorithm. Keeps a buffer with a list of least recently used to most recently used along with the buffer for memory. When there is a fault, the algorithm will replace the address that was the least recently used.
// FIFO: code for first-in-first-out algorithm. When there is a fault, the algorithm will replace the address that has been there the longest.
// CLOCK: code for clock policy. When there is a fault, the algorithm will replace the first addresses it encounters with a bit value of 0.
// RANDOM: code for random replacement algorithm. When there is a fault, the algorith will replace an address at random.

#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>

using namespace std;

//prototypes
int LRU(int, int *);
int FIFO(int, int *);
int CLOCK(int, int *);
int RANDOM(int, int *);

// generates memory traces, performs algorithm function calls, calculates and displays average number of faults
int main()
{
    int LRU_FAULTS_TOTAL[19];
    int FIFO_FAULTS_TOTAL[19];
    int CLOCK_FAULTS_TOTAL[19];
    int RANDOM_FAULTS_TOTAL[19];
    
    int base = 0;
    int *addresses = new int[1000];
    
    //to make the randomly generated integers more random
    long int seed = time(0);
    srand(seed);
    
    //initiate arrays
    for(int x = 0; x < 19; x++)
    {
        LRU_FAULTS_TOTAL[x] = 0;
        FIFO_FAULTS_TOTAL[x] = 0;
        CLOCK_FAULTS_TOTAL[x] = 0;
        RANDOM_FAULTS_TOTAL[x] = 0;
    }
    
    //start of main code
    for(int i = 0; i<1000; i++)
    {
        //generate memory trace
        for(int x = 0; x < 10; x++)
        {
            base = 25 * (rand() % 100);
            for(int y = 0; y < 100; y++)
            {
                addresses[100 * x + y] = base + (rand() % 50);
            }
        }
        
        //function calls for faults
        for(int x = 2; x <= 20; x++)
        {
            LRU_FAULTS_TOTAL[x-2] += LRU(x, addresses);
            FIFO_FAULTS_TOTAL[x-2] += FIFO(x, addresses);
            CLOCK_FAULTS_TOTAL[x-2] += CLOCK(x, addresses);
            RANDOM_FAULTS_TOTAL[x-2] += RANDOM(x, addresses);
        }
    }
    
    //display data
    for(int x = 2; x <= 20; x++)
    {
        cout << "Set " << x << endl;
        cout << "************************" << endl;
        cout << "LRU: " << (double)LRU_FAULTS_TOTAL[x-2]/1000 << endl;
        cout << "FIFO: " << (double)FIFO_FAULTS_TOTAL[x-2]/1000 << endl;
        cout << "CLOCK: " << (double)CLOCK_FAULTS_TOTAL[x-2]/1000 << endl;
        cout << "RANDOM: " << (double)RANDOM_FAULTS_TOTAL[x-2]/1000 << endl;
    }
    return 0;
}

// When there is a fault, the address that is least recently used will be replaced
int LRU(int workset, int *addresses)
{
    int faults = 0;
    int buffer[workset];
    int LRUBuffer[workset]; // used to determine which page is the least recently used in order
    bool pageFound = false;
    
    //initiate buffer
    for(int x = 0; x < workset; x++)
    {
        buffer[x] = -1;
    }
    
    
    for(int x = 0; x < 1000; x++)
    {
        pageFound = false;

        // if buffer is not full yet
        if(buffer[workset-1] == -1)
        {
            // loop through buffer to see if page exists
            for(int y = 0; y < workset; y++)
            {
                // if page exists
                if(buffer[y] == addresses[x])
                {
                    // set pageFound to true and move value up to the top in LRUBuffer
                    pageFound = true;
                    for(int z = 0; z < workset; z++)
                    {
                        // if the value in LRUBuffer matches the value in buffer
                        if (LRUBuffer[z] == buffer[y])
                        {
                            // move that value to the top and move the other values down in the buffer
                            for(int i = z; i < workset; i++)
                            {
                                if(buffer[i] == -1)
                                {
                                    LRUBuffer[i] = addresses[x];
                                }
                                else
                                {
                                    LRUBuffer[i] = LRUBuffer[i+1];
                                }
                            }
                        }
                    }
                }
            }
            
            // if the page does not exist
            if(pageFound == false)
            {
                // place the value in the buffers
                int y = 0;
                while(pageFound == false)
                {
                    if(buffer[y] == -1)
                    {
                        buffer[y] = addresses[x];
                        LRUBuffer[y] = addresses[x];
                        pageFound = true;
                    }
                    y++;
                }
            }
        }
        
        // if the buffer is already filled
        else
        {
            // loop through buffer
            for(int y = 0; y < workset; y++)
            {
                // if page is found in the buffer
                if(buffer[y] == addresses[x])
                {
                    // set pageFound to true and move page to the top of LRUBuffer
                    pageFound = true;
                    for(int z = 0; z < workset; z++)
                    {
                        if (LRUBuffer[z] == buffer[y])
                        {
                            for(int i = z; i < workset; i++)
                            {
                                LRUBuffer[i] = LRUBuffer[i+1];
                            }
                            LRUBuffer[workset - 1] = addresses[x];
                        }
                    }
                }
            }
            
            // if page is not in the buffer, increment faults and insert page into the buffer
            if(pageFound == false)
            {
                faults++;
                for(int i = 0; i < workset; i++)
                {
                    if(buffer[i] == LRUBuffer[0])
                    {
                        buffer[i] = addresses[x];
                    }
                }
                // rearrange LRUBuffer by taking first value out and insert new value on top
                for(int i = 0; i < workset - 1; i++)
                {
                    LRUBuffer[i] = LRUBuffer[i+1];
                }
                LRUBuffer[workset - 1] = addresses[x];
            }
        }
    }
    return faults;
}

        
// When there is a fault, the address that has been in the buffer the longest will be replaced
int FIFO(int workset, int *addresses)
{
    int faults = 0;
    int buffer[workset];
    int replace = 0; // used to determine the page that has been in the buffer the longest
    bool pageFound = false;
    
    // initiate buffer
    for(int x = 0; x < workset; x++)
    {
        buffer[x] = -1;
    }
    
    for(int x = 0; x < 1000; x++)
    {
        // if buffer is not full
        pageFound = false;
        if(buffer[workset-1] == -1)
        {
            // loop through buffer
            for(int y = 0; y < workset; y++)
            {
                // if page is found in buffer, set pageFound to true
                if(buffer[y] == addresses[x])
                {
                    pageFound = true;
                }
            }
            // if page is not found
            if(pageFound == false)
            {
                // insert page into buffer
                int y= 0;
                while(pageFound == false)
                {
                    if(buffer[y] == -1)
                    {
                        buffer[y] = addresses[x];
                        pageFound = true;
                    }
                    y++;
                }
            }
        }
        
        // if buffer is full
        else
        {
            // loop through buffer
            for(int y = 0; y < workset; y++)
            {
                // if page is found in buffer, set pageFound to true
                if(buffer[y] == addresses[x])
                {
                    pageFound = true;
                }
            }
            
            // if page is not found in buffer, increment fault and place page into buffer
            if(pageFound == false)
            {
                faults++;
                buffer[replace] = addresses[x];
                replace++;
                if(replace >= workset)
                {
                    replace = 0;
                }
            }
        }
    }
    return faults;
}

// When there is a fault, the algorithm will replace the first address it encounters with a bit value of 0
int CLOCK(int workset, int *addresses)
{
    int faults = 0;
    int pointer = 0;
    int buffer[workset];
    int bitArray[workset]; // used to determine the bit (0 or 1) of each page in the buffer
    bool pageFound = false;
    
    // initiate buffer
    for(int x = 0; x < workset; x++)
    {
        buffer[x] = -1;
    }
    
    for(int x = 0; x < 1000; x++)
    {
        // if buffer is not full
        pageFound = false;
        if(buffer[workset-1] == -1)
        {
            // loop through buffer
            for(int y = 0; y < workset; y++)
            {
                // if page is found, set pageFound to true and set the bit of the page to 1
                if(buffer[y] == addresses[x])
                {
                    bitArray[y] = 1;
                    pageFound = true;
                }
            }
            
            // if page is not found
            if(pageFound == false)
            {
                // insert page into the buffer and set bit to 1
                int y = 0;
                while(pageFound == false)
                {
                    if(buffer[y] == -1)
                    {
                        buffer[y] = addresses[x];
                        bitArray[y] = 1;
                        pointer++;
                        if(pointer >= workset)
                        {
                            pointer = 0;
                        }
                        pageFound = true;
                    }
                    y++;
                }
            }
        }
        
        // if buffer is not full
        else
        {
            // loop through buffer
            for(int y = 0; y < workset; y++)
            {
                // if page is found in buffer, set pageFound to true and set bit to 1
                if(buffer[y] == addresses[x])
                {
                    pageFound = true;
                    bitArray[y] = 1;
                }
            }
            
            // if page is not found, increment faults
            if(pageFound == false)
            {
                faults++;
            }
            
            // move around the clock until the page is inserted into the buffer
            while(pageFound == false)
            {
                if((buffer[pointer] != addresses[x]) && (bitArray[pointer] == 1))
                {
                    bitArray[pointer] = 0;
                    pointer++;
                    if(pointer >= workset)
                    {
                        pointer = 0;
                    }
                }
                else if((buffer[pointer] != addresses[x]) && (bitArray[pointer] == 0))
                {
                    buffer[pointer] = addresses[x];
                    bitArray[pointer] = 1;
                    pointer++;
                    if(pointer >= workset)
                    {
                        pointer = 0;
                    }
                    pageFound = true;
                }
            }
        }
    }
    return faults;
}

// When there is a page fault, the algorithm will replace an address at random
int RANDOM(int workset, int *addresses)
{
    int faults = 0;
    int buffer[workset];
    bool pageFound = false;
    
    // initialize buffer
    for(int x = 0; x < workset; x++)
    {
        buffer[x] = -1;
    }
    
    for(int x = 0; x < 1000; x++)
    {
        // if buffer is not full
        pageFound = false;
        if(buffer[workset-1] == -1)
        {
            // loop through buffer
            for(int y = 0; y < workset; y++)
            {
                // if page is found in the buffer, set pageFound to true
                if(buffer[y] == addresses[x])
                {
                    pageFound = true;
                }
            }
            
            // if page is not found
            if(pageFound == false)
            {
                // insert page into buffer
                int y = 0;
                while(pageFound == false)
                {
                    if(buffer[y] == -1)
                    {
                        buffer[y] = addresses[x];
                        pageFound = true;
                    }
                    y++;
                }
            }
        }
        
        // if buffer is not full
        else
        {
            // loop through buffer
            for(int y = 0; y < workset; y++)
            {
                // if page is found, set pageFound to true
                if(buffer[y] == addresses[x])
                {
                    pageFound = true;
                }
            }
            
            // if page is not found, increment faults and replace a random page in buffer
            if(pageFound == false)
            {
                faults++;
                buffer[rand() % workset] = addresses[x];
            }
        }
    }
    return faults;
}


