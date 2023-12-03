#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <../graphique/ProcessesInterface.h>

// Global variables
int outputIndex = 0;
int output[100];
int time_quantum; // Variable to store the time quantum

// Constants
#define MAX_PROCESSES 100 // Define the maximum number of processes

// Function declarations
void displayProcessStats(Process tab2[], int n);
void Round_Robin(Process tab2[], int n);
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

    // Create an array to store information about each process
    Process tab2[50];
    int np = 0;

    // Read process information from the file and populate the array
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        if (np >= 50) {
            printf("Too many processes!\n");
            break;
        }

        int arrive_time, burst, priority;
        char id[ID_LEN];

        // Validate and read process details
        if (sscanf(line, "%s %d %d %d", id, &arrive_time, &burst, &priority) == 4) {
            strcpy(tab2[np].id, id);
            tab2[np].arrive_time = arrive_time;
            tab2[np].burst = burst;
            tab2[np].priority = priority;
            tab2[np].completed = 0;
            np++;
        }
    }

    // Close the file
    fclose(fp);

    // Perform Round Robin Scheduling
    Round_Robin(tab2, np);

    // Display process statistics
    displayProcessStats(tab2, np);

    // Display the Gantt chart
    Gantt_Chart(tab2, np);

    // Call the function to display Gantt chart in GTK window
    display_roundrobin_interface(tab2, output, outputIndex, time_quantum);

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
        int turnaroundTime = tab2[i].return_time - tab2[i].arrive_time;
        int waitingTime = turnaroundTime - tab2[i].execution_time;

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        printf("%s\t%d\t\t%d\n", tab2[i].id, waitingTime, turnaroundTime);
    }

    printf("\n");
    printf("(TRM) Average Waiting Time: %.2f\n", (float)totalWaitingTime / n);
    printf("(TAM) Average Turnaround Time: %.2f\n", (float)totalTurnaroundTime / n);
    printf("\n");
    printf("\n");
}

// Function for Round Robin Scheduling
void Round_Robin(Process tab2[], int n) {
    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    // Initialize the execution time for each process
    for (int i = 0; i < n; i++) {
        // Contain the initial burst time input
        tab2[i].execution_time = tab2[i].burst;
    }

    // Display scheduling information
    printf(" **** Process Scheduling ****\n");
    printf("Process\tArrival Time\tExecution Time\tCompletion Time\n");

    int time = 0;
    int num_processes = n;
    int completed = 0;

    // Perform Round Robin Scheduling
    while (completed < num_processes) {
        for (int i = 0; i < n; i++) {
            if (!tab2[i].completed) {
                if (tab2[i].burst > 0) {
                    if (tab2[i].burst > time_quantum) {
                        // Execute process for a time quantum
                        printf("%s\t%d\t\t%d\t\t", tab2[i].id, tab2[i].arrive_time, tab2[i].execution_time);
                        tab2[i].completion_time = time + time_quantum;
                        printf("%d (Quantum expired)\n", tab2[i].completion_time);

                        // Update process information
                        output[outputIndex++] = i + 1;
                        tab2[i].burst -= time_quantum;
                        time += time_quantum;
                    } else {
                        // Execute remaining burst time for the process
                        printf("%s\t%d\t\t%d\t\t", tab2[i].id, tab2[i].arrive_time, tab2[i].execution_time);
                        time += tab2[i].burst;
                        tab2[i].burst = 0;
                        completed++;
                        tab2[i].return_time = time;

                        // Update process information
                        tab2[i].completion_time = time;
                        printf("%d (Completed)\n", tab2[i].completion_time);
                        output[outputIndex++] = i + 1;
                        tab2[i].completed = 1;
                    }
                }
            }
        }
    }
}

// Function to display the Gantt chart
void Gantt_Chart(Process tab2[], int n) {
    // Display the order of process execution
    printf("Order of Execution: ");
    printf("\n---------- ------  ------  ------  ------  ------  ------  ------ --\n");
    for (int i = 0; i < outputIndex; i++) {
        printf("|    p%d   ", output[i]);
    }
    printf("|\n---------- ------  ------  ------  ------  ------  ------  ------ --\n");
}

