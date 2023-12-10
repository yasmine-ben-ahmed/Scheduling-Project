#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <../graphique/ProcessesInterface.h>

int time_quantum;

void displayProcessStats_rr(Process tab2[], int n);
void Round_Robin(Process tab2[], int n);
void Gantt_Chart(Process tab2[], int n);
int compareArrivalTimerr(const void* a, const void* b);
void sortByArrivalTimerr(Process tab2[], int n);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    Process tab2[50];
    int np = 0;
    char line[500];

    while (fgets(line, sizeof(line), fp)) {
        if (np >= 50) {
            printf("Too many processes!\n");
            break;
        }

        int arrive_time, burst, priority;
        char id[ID_LEN];

        if (sscanf(line, "%s %d %d %d", id, &arrive_time, &burst, &priority) == 4) {
            strcpy(tab2[np].id, id);
            tab2[np].arrive_time = arrive_time;
            tab2[np].burst = burst;
            tab2[np].priority = priority;
            tab2[np].completed = 0;
            np++;
        }
    }

    fclose(fp);

    Round_Robin(tab2, np);
    displayProcessStats_rr(tab2, np);
    Gantt_Chart(tab2, np);

    printf("\n");

    display_roundrobin_interface(tab2, output, outputIndex, np, time_quantum);

    return 0;
}

void displayProcessStats_rr(Process tab2[], int n) {
    printf("\nProcess\tArrival Time\tWaiting Time\tTurnaround Time\n");
    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    for (int i = 0; i < n; i++) {
        int turnaroundTime = tab2[i].return_time - tab2[i].arrive_time;
        int waitingTime = turnaroundTime - tab2[i].execution_time;

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        printf("%s\t%d\t\t%d\t\t%d\n", tab2[i].id, tab2[i].arrive_time, waitingTime, turnaroundTime);
    }

    printf("\n(TRM) Average Waiting Time: %.2f\n", (float)totalWaitingTime / n);
    printf("(TAM) Average Turnaround Time: %.2f\n", (float)totalTurnaroundTime / n);
    printf("\n\n");
}

int compareArrivalTimerr(const void* a, const void* b) {
    Process* processA = (Process*)a;
    Process* processB = (Process*)b;
    return (processA->arrive_time - processB->arrive_time);
}

void sortByArrivalTimerr(Process tab2[], int n) {
   qsort(tab2, n, sizeof(Process), compareArrivalTimerr);
}

void Round_Robin(Process tab2[], int n) {
    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    for (int i = 0; i < n; i++) {
        tab2[i].execution_time = tab2[i].burst;
    }

    sortByArrivalTimerr(tab2, n);

    printf("**** Process Scheduling Round Robin algorithm****\n");
    printf("Process\tArrival Time\tExecution Time\tCompletion Time\n");

    int time = 0;
    int num_processes = n;
    int completed = 0;

    while (completed < num_processes) {
        for (int i = 0; i < n; i++) {
            if (!tab2[i].completed ) {
                if (tab2[i].burst > 0) {
                    if (tab2[i].burst > time_quantum ) {
                        printf("%s\t%d\t\t%d\t\t", tab2[i].id, tab2[i].arrive_time, tab2[i].execution_time);
                        tab2[i].completion_time = time + time_quantum;
                        printf("%d (Quantum expired)\n", tab2[i].completion_time);

                        output[outputIndex++] = i + 1;
                        tab2[i].burst -= time_quantum;
                        time += time_quantum;
                    } else {
                        printf("%s\t%d\t\t%d\t\t", tab2[i].id, tab2[i].arrive_time, tab2[i].execution_time);
                        time += tab2[i].burst;
                        tab2[i].burst = 0;
                        completed++;
                        tab2[i].return_time = time;

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

void Gantt_Chart(Process tab2[], int n) {
    printf("Order of Execution: ");
    printf("\n----------------------------------\n");
    for (int i = 0; i < outputIndex; i++) {
        printf("|    p%d   ", output[i]);
    }
    printf("|\n----------------------------------\n");
}
