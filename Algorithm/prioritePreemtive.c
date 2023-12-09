#include <stdio.h>
#include <stdlib.h>

#include <../graphique/ProcessesInterface.h>

#include "priorityprem.h"

#define MAX_PROCESS_COUNT 500 // Define an appropriate maximum value

// Global variables
int outputIndex = 0;
int output[MAX_PROCESS_COUNT]; // Store indices instead of IDs

void performPriorityScheduling(Process *process, int process_count) {
    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int completed_processes = 0;
    int i;


    printf("\n---Gantt Chart:\n");
    while (completed_processes < process_count) {
        int selected_process = find_highest_priority(process, process_count, current_time);

        if (selected_process == -1) {
            current_time++;
            continue;
        }

        printf("| %s ", process[selected_process].id);
                                // Update process information
        output[outputIndex++] = selected_process + 1 ; // Store the index


        if (process[selected_process].response_time == 0) {
            process[selected_process].response_time = current_time - process[selected_process].arrive_time;

        }

        process[selected_process].waiting_time += current_time - process[selected_process].return_time;
        process[selected_process].return_time = current_time + 1;
        process[selected_process].burst--;

        if (process[selected_process].burst == 0) {
            process[selected_process].completed = TRUE;
            process[selected_process].turnaround_time = current_time - process[selected_process].arrive_time + 1;
            total_waiting_time += process[selected_process].waiting_time;
            total_turnaround_time += process[selected_process].turnaround_time;




            completed_processes++;
        }

        current_time++;
    }
    
     printf("\n");
    
        // Print the content of the output array
  /*  printf("inside function Contents of the 'output' array:\n");
    for (int j = 0; j < outputIndex; j++) {
        printf("%d ", output[j]);
    }
    printf("\n");*/
    

    // Print table
    puts(""); // Empty line
    print_table(process, process_count);

    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", total_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)total_waiting_time / (double)process_count);
    printf("Total Turnaround Time   : %-2d\n", total_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)total_turnaround_time / (double)process_count);
}


int main(int argc, char *argv[]) {
    int i;
    Process *process;
    int process_count = 0;

    if (argc < 2 || argc > 2 || argv[1] == NULL) {
        printf("Invalid arguments!\n");
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

        // Store the initial burst time in a separate variable 'execution_time'
        int execution_time = process[i].burst;
        process[i].execution_time = execution_time; // Store initial burst time in the structure
                output[i] = i; // Store the index
        i++;
    }
    fclose(fp);

    printf("Number of processes = %d\n", process_count);

    process_init(process, process_count);
    performPriorityScheduling(process, process_count);
    

     printf("\n");
     
    // Print the content of the output array
   /* printf("Contents of the 'output' array:\n");
    for (int j = 0; j < outputIndex; j++) {
        printf("%d ", output[j]);
    }
    printf("\n");*/
    
    

    // Call the function to display the priority scheduling interface
    display_prprem_interface(process, output, outputIndex,process_count);

    free(process);

    return 0;
}

