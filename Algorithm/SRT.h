#include <graphics.h>

// Function prototype for finding the index of the process with the shortest remaining time
int findShortestRemainingTimeIndex(Process processes[], int n, int currentTime);



// Function to find the index of the process with the shortest remaining time
int findShortestRemainingTimeIndex(Process processes[], int n, int currentTime) {
    int shortestIndex = -1;
    int shortestTime = INT_MAX;

    // Iterate through the processes
    for (int i = 0; i < n; i++) {
        // Check if the process has arrived, has remaining time, and has shorter remaining time
        if (processes[i].arrive_time <= currentTime &&
            processes[i].remaining_time < shortestTime &&
            processes[i].remaining_time > 0) {
            shortestIndex = i;
            shortestTime = processes[i].remaining_time;
        }
    }

    return shortestIndex;
}


// Function to swap two processes
void swap(Process *a, Process *b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

// Function to sort processes by arrival time
void sortByArrivalTime(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrive_time > processes[j + 1].arrive_time) {
                swap(&processes[j], &processes[j + 1]);
            }
        }
    }
}



void SRT(Process processes[], int n) {
    sortByArrivalTime(processes, n); // Sort processes by arrival time

    int currentTime = processes[0].arrive_time;

    printf("Sequence of processes in SRT: ");

    while (1) {
        int index = findShortestRemainingTimeIndex(processes, n, currentTime);

        if (index == -1) {
            break; // All processes have been completed
        }

        printf("%s ", processes[index].id);
        

        output[outputIndex++] = index; // Store the index

      

        processes[index].remaining_time--;

        if (processes[index].remaining_time == 0) {
            processes[index].completion_time = currentTime + 1;
        }

        currentTime++;
    }
    
}


