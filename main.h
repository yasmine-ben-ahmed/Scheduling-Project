#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int input() {
    usleep(1000);
    srand(time(NULL)); 

    FILE *fp = fopen("inputs.txt", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(fp, "4\n");
    printf("Number of processes generated randomly: 4\n");

    for (int i = 1; i <= 4; i++) {
        int arrival_time =  rand() % 10;
        int burst_time = 1+  rand() % 10;
        int priority = 1+ rand() % 10;

        fprintf(fp, "P%d %d %d %d\n", i, arrival_time, burst_time, priority);
        printf("P%d %d %d %d\n", i, arrival_time, burst_time, priority);
    }

    fclose(fp);
    return 0;
}

