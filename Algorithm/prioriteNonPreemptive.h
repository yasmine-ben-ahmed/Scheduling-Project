#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void displayProcessStats_pr(Process tab2[], int n);
void PrioritéNonP(Process tab2[], int n);
void Gantt_Chart(Process tab2[], int n);

void displayProcessStats_pr(Process tab2[], int n) {
    printf("\n");
    printf("\n");
    printf("Process\tWaiting Time\tTurnaround Time\n");

    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    for (int i = 0; i < n; i++) {
        int turnaroundTime = tab2[i].tempsfin - tab2[i].arrive_time;
        int waitingTime = turnaroundTime - tab2[i].burst;

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        printf("%s\t%d\t\t%d\n", tab2[i].id, waitingTime, turnaroundTime);
    }

    printf("\n");
    printf("\n");

    printf("(TAM) Average Waiting Time: %.2f\n", (float)totalWaitingTime / n);
    printf("(TRM) Average Turnaround Time: %.2f\n", (float)totalTurnaroundTime / n);
}

void PrioritéNonP(Process tab2[], int n) {
    int currentTime = 0;

    printf(" **** Priority Process Scheduling without preemption****\n");
    printf("Process\tArrival Time\tExecution Time\tPriority\tCompletion Time\n");

    int completedProcesses = 0;

    while (completedProcesses < n) {
        int highestPriorityIndex = -1;
        int highestPriority = INT_MIN;

        for (int j = 0; j < n; j++) {
            if (tab2[j].reste > 0 && tab2[j].arrive_time <= currentTime && tab2[j].priority > highestPriority) {
                highestPriority = tab2[j].priority;
                highestPriorityIndex = j;
            }
        }

        if (highestPriorityIndex == -1) {
            int nextProcessTime = INT_MAX;

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

            printf("%s\t%d\t\t%d\t\t%d\t\t%d\n", tab2[currentProcessIndex].id, tab2[currentProcessIndex].arrive_time, tab2[currentProcessIndex].burst, tab2[currentProcessIndex].priority, tab2[currentProcessIndex].tempsfin);

            completedProcesses++;
            output[outputIndex++] = currentProcessIndex;
        }
    }
}

void Gantt_Chart(Process tab2[], int n) {
    printf("Gantt Chart:\n");
    printf("---------------------------------------------\n");
    printf("|");
    for (int i = 0; i < outputIndex; i++) {
        printf("  %s  |", tab2[output[i]].id);
    }
    printf("\n---------------------------------------------\n");
}

