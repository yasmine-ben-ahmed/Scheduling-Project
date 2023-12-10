#include <stdio.h>
#include <stdlib.h>

// Include the necessary header file for graphique/ProcessesInterface
#include <../graphique/ProcessesInterface.h>

// Include the local header file priorityprem.h
#include "priorityprem.h"

int main(int argc, char *argv[]) {
    int i = 0; // Initialize i to zero
    Process *process; // Declare a pointer to Process struct
    int process_count = 0;

    // Check if the correct number of arguments is provided
    if (argc != 2 || argv[1] == NULL) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Open the file for reading
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("FILE OPEN ERROR!\n");
        return 1;
    }

    // Read the number of processes from the file
    fscanf(fp, " %d", &process_count);
    process = (Process *)malloc(sizeof(Process) * process_count);

    // Read process details from the file and store them in the process array
    while (i < process_count) {
        fscanf(fp, "%s %d %d %d", process[i].id, &process[i].arrive_time, &process[i].burst, &process[i].priority);

        // Store the initial burst time in a separate variable 'execution_time'
        int execution_time = process[i].burst;
        process[i].execution_time = execution_time; // Store initial burst time in the structure
        
        // Increment index and continue reading the next process
        i++;
    }
    fclose(fp);

    printf("Number of processes = %d\n", process_count);

    // Initialize the processes
    process_init(process, process_count);

    // Perform priority scheduling
    performPriorityScheduling(process, process_count);

    printf("\n");

    // Assuming 'output', 'outputIndex', and 'display_prprem_interface' are declared somewhere
    // Call the function to display the priority scheduling interface
    display_prprem_interface(process, output, outputIndex, process_count);

    // Free dynamically allocated memory
    free(process);

    return 0;
}

