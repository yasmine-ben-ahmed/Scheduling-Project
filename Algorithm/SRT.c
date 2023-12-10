#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <../graphique/ProcessesInterface.h>

void displayProcessStats_srt(Process processes[], int n);

int main(int argc, char* argv[]) {
    int n;

    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    fscanf(fp, "%d", &n);

    Process *tab2 = (Process *)malloc(n * sizeof(Process));

    if (tab2 == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

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
        tab2[i].remaining_time = burst_time;
    }

    fclose(fp);

    sortByArrivalTime(tab2, n);

    SRT(tab2, n);

    displayProcessStats_srt(tab2, n);

    printf("\n srt.c  pb before display_order_of_execution_srt function"); 
    display_SRT_interface(tab2, n, output, outputIndex);
    
    return 0;
}

void displayProcessStats_srt(Process processes[], int n) {
    printf("\n");
    printf("Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    for (int i = 0; i < n; i++) {
        int turnaroundTime = processes[i].completion_time - processes[i].arrive_time;
        int waitingTime = turnaroundTime - processes[i].burst;

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        printf("%s\t%d\t\t%d\t\t%d\t\t%d\n",
            processes[i].id,
            processes[i].arrive_time,
            processes[i].burst,
            waitingTime,
            turnaroundTime
        );
    }

    printf("\n(TRM) Average Waiting Time: %.2f\n", (float)totalWaitingTime / n);
    printf("(TAM) Average Turnaround Time: %.2f\n", (float)totalTurnaroundTime / n);
    printf("\n");
}

