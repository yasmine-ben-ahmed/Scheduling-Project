#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <../graphique/ProcessesInterface.h>

// Global variables
int output[20]; // Record the sequence of executed processes
int outputIndex = 0; // Index for the output array

// Function declarations
void displayProcessStats(Process tab2[], int n);
void PrioritéNonP(Process tab2[], int n);
void Gantt_Chart(Process tab2[], int n);

int main(int argc, char *argv[]) {
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

    // Read the number of processes from the file
    int n;
    fscanf(fp, "%d", &n);

    // Create an array to store information about each process
    Process tab2[n];

    // Read process information from the file and populate the array
    for (int i = 0; i < n; i++) {
        int arrive_time, burst, priority;
        char id[ID_LEN];

        // Validate and read process details
        if (fscanf(fp, "%s %d %d %d", id, &arrive_time, &burst, &priority) != 4) {
            printf("Invalid input format.\n");
            return 1;
        }

        // Populate the process structure
        strcpy(tab2[i].id, id);
        tab2[i].arrive_time = arrive_time;
        tab2[i].burst = burst;
        tab2[i].priority = priority;
        tab2[i].reste = burst;
    }

    // Close the file
    fclose(fp);

    // Perform Priority Non-Preemptive Scheduling
    PrioritéNonP(tab2, n);

    // Display process statistics
    displayProcessStats(tab2, n);

    // Display the Gantt chart
    Gantt_Chart(tab2, n);

    // Call the function to display Gantt chart in GTK window
    display_prNonp_interface(tab2, output, outputIndex);

    return 0;
}

// Function to display process statistics
void displayProcessStats(Process tab2[], int n) {
    printf("\n");
    printf("\n");
    printf("Process\tWaiting Time\tTurnaround Time\n");

    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    // Calculate and display waiting and turnaround times for each process
    for (int i = 0; i < n; i++) {
        int turnaroundTime = tab2[i].tempsfin - tab2[i].arrive_time;
        int waitingTime = turnaroundTime - tab2[i].burst;

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        printf("%s\t%d\t\t%d\n", tab2[i].id, waitingTime, turnaroundTime);
    }

    printf("\n");
    printf("\n");

    // Display average waiting and turnaround times
    printf("(TAM) Average Waiting Time: %.2f\n", (float)totalWaitingTime / n);
    printf("(TRM) Average Turnaround Time: %.2f\n", (float)totalTurnaroundTime / n);
}

// Function for Priority Non-Preemptive Scheduling
void PrioritéNonP(Process tab2[], int n) {
    int currentTime = tab2[0].arrive_time; // Set the initial time to the arrival time of the first process

    printf(" **** Priority Process Scheduling ****\n");
    printf("Process\tArrival Time\tExecution Time\tPriority\tCompletion Time\n");

    // Continue scheduling until all processes are completed
    while (1) {
        int highestPriorityIndex = -1;
        int highestPriority = -1; // Initialize with lowest possible priority

        // Find the highest priority process that has arrived and not yet completed
        for (int j = 0; j < n; j++) {
            if (tab2[j].reste > 0 && tab2[j].arrive_time <= currentTime && tab2[j].priority > highestPriority) {
                highestPriority = tab2[j].priority;
                highestPriorityIndex = j;
            }
        }

        if (highestPriorityIndex == -1) {
            int nextProcessTime = 9999; // Set a large value as default

            // Find the next process arrival time
            for (int j = 0; j < n; j++) {
                if (tab2[j].reste > 0 && tab2[j].arrive_time > currentTime && tab2[j].arrive_time < nextProcessTime) {
                    nextProcessTime = tab2[j].arrive_time;
                }
            }

            if (nextProcessTime == 9999) {
                // All processes are completed
                break;
            } else {
                currentTime = nextProcessTime;
            }
        } else {
            int currentProcessIndex = highestPriorityIndex;
            int executionTime = tab2[currentProcessIndex].reste;

            currentTime += executionTime;
            tab2[currentProcessIndex].reste = 0;
            tab2[currentProcessIndex].tempsfin = currentTime;

            // Print details of the executed process
            printf("%s\t%d\t\t%d\t\t%d\t\t%d\t\t", tab2[currentProcessIndex].id, tab2[currentProcessIndex].arrive_time, tab2[currentProcessIndex].burst, tab2[currentProcessIndex].priority, tab2[currentProcessIndex].tempsfin);
            printf("\n");

            // Record the executed process index
            output[outputIndex++] = currentProcessIndex;
        }
    }
}

// Function to display the Gantt chart
void Gantt_Chart(Process tab2[], int n) {
    // Print the Gantt chart based on the execution sequence
    printf("Gantt Chart:\n");
    printf("---------------------------------------------\n");
    printf("|");
    for (int i = 0; i < outputIndex; i++) {
        printf("  %s  |", tab2[output[i]].id);
    }
    printf("\n---------------------------------------------\n");
}

