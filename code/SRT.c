#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <../graphique/ProcessesInterface.h>

// Function declarations
void displayProcessStats_srt(Process processes[], int n);

// Main function
int main(int argc, char* argv[]) {
    int n;

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
    int *output = malloc(n * sizeof(int));  // Allocate memory for output array

    if (output == NULL) {
        printf("Memory allocation failed.\n");
        free(tab2);  // Free memory allocated for tab2 before exiting
        return 1;
    }

    int outputIndex = 0;
    SRT(tab2, n, output, &outputIndex);  // Pass output array and its index

    // Display process statistics in the console
    displayProcessStats_srt(tab2, n);

    // Display the SRT interface and the sequence of processes
    display_SRT_interface(tab2, n, output, outputIndex);

    // Free allocated memory
    free(output);  // Free memory allocated for output array
    free(tab2);
    return 0;
}


// Function to display process statistics
void displayProcessStats_srt(Process processes[], int n) {
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
