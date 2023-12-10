#include <stdio.h>
#include <stdlib.h>
#include <../graphique/ProcessesInterface.h>
#include "priorityprem.h"

int main(int argc, char *argv[]) {
    int i = 0;
    Process *process;
    int process_count = 0;

    if (argc != 2 || argv[1] == NULL) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("FILE OPEN ERROR!\n");
        return 1;
    }

    fscanf(fp, " %d", &process_count);
    process = (Process *)malloc(sizeof(Process) * process_count);

    while (i < process_count) {
        fscanf(fp, "%s %d %d %d", process[i].id, &process[i].arrive_time, &process[i].burst, &process[i].priority);

        int execution_time = process[i].burst;
        process[i].execution_time = execution_time;
        i++;
    }
    fclose(fp);

    printf("Number of processes = %d\n", process_count);

    process_init(process, process_count);
    performPriorityScheduling(process, process_count);

    printf("\n");

    display_prprem_interface(process, output, outputIndex, process_count);

    free(process);

    return 0;
}

