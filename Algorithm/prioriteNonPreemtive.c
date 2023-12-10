#include <../graphique/ProcessesInterface.h>
#include "prioriteNonPreemptive.h"

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

    int n;
    fscanf(fp, "%d", &n);

    Process tab2[n];

    for (int i = 0; i < n; i++) {
        int arrive_time, burst, priority;
        char id[ID_LEN];

        if (fscanf(fp, "%s %d %d %d", id, &arrive_time, &burst, &priority) != 4) {
            printf("Invalid input format.\n");
            return 1;
        }

        strcpy(tab2[i].id, id);
        tab2[i].arrive_time = arrive_time;
        tab2[i].burst = burst;
        tab2[i].priority = priority;
        tab2[i].reste = burst;
    }

    fclose(fp);

    PrioritéNonP(tab2, n);

    displayProcessStats_pr(tab2, n);

    Gantt_Chart(tab2, n);

    display_prNonp_interface(tab2, output, outputIndex);

    return 0;
}

