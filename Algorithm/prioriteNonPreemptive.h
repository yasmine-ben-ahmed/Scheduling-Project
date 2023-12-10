#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function declarations
void displayProcessStats_pr(Process tab2[], int n);
void PrioritéNonP(Process tab2[], int n);
void Gantt_Chart(Process tab2[], int n);


// Function to display process statistics
void displayProcessStats_pr(Process tab2[], int n) {
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
    int currentTime = 0;

    printf(" **** Priority Process Scheduling without preemption****\n");
    printf("Process\tArrival Time\tExecution Time\tPriority\tCompletion Time\n");

    int completedProcesses = 0;

    while (completedProcesses < n) {
        int highestPriorityIndex = -1;
        int highestPriority = INT_MIN;

        // Find the highest priority process that has arrived and not yet completed
        for (int j = 0; j < n; j++) {
            if (tab2[j].reste > 0 && tab2[j].arrive_time <= currentTime && tab2[j].priority > highestPriority) {
                highestPriority = tab2[j].priority;
                highestPriorityIndex = j;
            }
        }

        if (highestPriorityIndex == -1) {
            int nextProcessTime = INT_MAX;

            // Find the next process arrival time
            for (int j = 0; j < n; j++) {
                if (tab2[j].reste > 0 && tab2[j].arrive_time > currentTime && tab2[j].arrive_time < nextProcessTime) {
                    nextProcessTime = tab2[j].arrive_time;
                }
            }

            currentTime = nextProcessTime;
        } else {
            int currentProcessIndex = highestPriorityIndex;
            int executionTime = tab2[currentProcessIndex].reste;

            currentTime += executionTime;
            tab2[currentProcessIndex].reste = 0;
            tab2[currentProcessIndex].tempsfin = currentTime;

            // Print details of the executed process
            printf("%s\t%d\t\t%d\t\t%d\t\t%d\n", tab2[currentProcessIndex].id, tab2[currentProcessIndex].arrive_time, tab2[currentProcessIndex].burst, tab2[currentProcessIndex].priority, tab2[currentProcessIndex].tempsfin);

            completedProcesses++;
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

