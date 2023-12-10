#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate and write random process data to a file
int input() {
    usleep(1000); // Sleep for 1 millisecond
    srand(time(NULL)); // Seed the random number generator

    // Open the file for writing
    FILE *fp = fopen("inputs.txt", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Write the number of processes
    fprintf(fp, "4\n");
    printf("Number of processes: 4\n");

    // Generate and write random values for each process
    for (int i = 1; i <= 4; i++) {
        int arrival_time = rand() % 10;
        int burst_time = 1 + rand() % 10; // Random burst time (1 to 10)
        int priority = 1 + rand() % 10; // Random priority (1 to 10)

        fprintf(fp, "P%d %d %d %d\n", i, arrival_time, burst_time, priority);
       // printf("P%d %d %d %d\n", i, arrival_time, burst_time, priority);
    }

    fclose(fp);
    return 0;
}

