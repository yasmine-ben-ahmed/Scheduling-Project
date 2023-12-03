#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <../graphique/ProcessesInterface.h>

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

// Function to display process statistics
void displayProcessStats(Process processes[], int n) {
    printf("\n");
    printf("\n");

    printf("Process\tWaiting Time\tTurnaround Time\n");
    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    // Calculate and display waiting and turnaround times for each process
    for (int i = 0; i < n; i++) {
        int turnaroundTime = processes[i].completion_time - processes[i].arrive_time;
        int waitingTime = turnaroundTime - processes[i].burst;

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        printf("%s\t%d\t\t%d\n", processes[i].id, waitingTime, turnaroundTime);
    }
    
    printf("\n");
    printf("\n");

    // Display average waiting and turnaround times
    printf("(TRM) Average Waiting Time: %.2f\n", (float)totalWaitingTime / n);
    printf("(TAM) Average Turnaround Time: %.2f\n", (float)totalTurnaroundTime / n);

    printf("\n");
}

// Function to perform Shortest Remaining Time First (SRTF) scheduling
void SRT(Process processes[], int n) {
    int currentTime = 0;

    printf("Sequence of processes: ");

    while (1) {
        int index = findShortestRemainingTimeIndex(processes, n, currentTime);

        if (index == -1) {
            break; // All processes have been completed
        }

        printf("%s ", processes[index].id);

        processes[index].remaining_time--;

        if (processes[index].remaining_time == 0) {
            processes[index].completion_time = currentTime + 1;
        }

        currentTime++;
    }
    
    displayProcessStats(processes, n);
}

// Main function
int main(int argc, char* argv[]) {
    int n;

    // Global variables to record the sequence of executed processes
    int output[n];
    int outputIndex = 0; // Index for output array

    gtk_init(&argc, &argv);
    
    // Check for the correct number of command-line arguments
    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the number of processes
    fscanf(fp, "%d", &n);

    // Dynamic memory allocation for an array of processes
    Process *tab2 = (Process *)malloc(n * sizeof(Process));

    if (tab2 == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Read process details from the file and populate the array
    for (int i = 0; i < n; i++) {
        int arrival_time, burst_time, priority;
        char process_id[ID_LEN];

        if (fscanf(fp, "%s %d %d %d", process_id, &arrival_time, &burst_time, &priority) != 4) {
            printf("Invalid input format.\n");
            return 1;
        }

        strcpy(tab2[i].id, process_id);
        tab2[i].arrive_time = arrival_time;
        tab2[i].burst = burst_time;
        tab2[i].priority = priority;
        tab2[i].remaining_time = burst_time; // Initialize remaining time with burst time
    }

    // Close the file
    fclose(fp);

    // Sort processes by arrival time
    sortByArrivalTime(tab2, n);

    // Perform Shortest Remaining Time First (SRTF) scheduling
    SRT(tab2, n);

    // Display the SRT interface
    display_SRT_interface(tab2, n, output, outputIndex);

    // Free allocated memory
    free(tab2);

    return 0;
}

