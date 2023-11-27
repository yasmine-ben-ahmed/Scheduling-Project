#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <../graphique/PrioritéNon_Inter.h>
#include <../graphique/ProcessesInterface.h>



int main(int argc, char* argv[]) {
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
    fscanf(fp, "%d", &n); // Read the number of processes

    Process tab2[n]; // Assuming the number of processes is provided in the file

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
        tab2[i].num = i + 1;
        tab2[i].waiting_time = 0;
        tab2[i].reste = burst;
    }

    fclose(fp);

    int currentTime = tab2[0].arrive_time; // Set the initial time to the arrival time of the first process

    printf(" **** Priority Process Scheduling ****\n");
    printf("Process\tArrival Time\tExecution Time\tPriority\tCompletion Time\n");

    int output[n]; // Record the sequence of executed processes
    int outputIndex = 0; // Index for output array

    while (1) {
        int highestPriorityIndex = -1;
        int highestPriority = -1; // Initialize with lowest possible priority

        // Find the highest priority process that has arrived and not yet completed
        for (int j = 0; j < n; j++) {
            if (tab2[j].reste > 0 && tab2[j].arrive_time <= currentTime && tab2[j].priority > highestPriority) {
                highestPriority = tab2[j].priority;
                highestPriorityIndex = j;
            }
        }

        if (highestPriorityIndex == -1) {
            int nextProcessTime = 9999; // Set a large value as default

            // Find the next process arrival time
            for (int j = 0; j < n; j++) {
                if (tab2[j].reste > 0 && tab2[j].arrive_time > currentTime && tab2[j].arrive_time < nextProcessTime) {
                    nextProcessTime = tab2[j].arrive_time;
                }
            }

            if (nextProcessTime == 9999) {
                // All processes are completed
                break;
            } else {
                currentTime = nextProcessTime;
            }
        } else {
            int currentProcessIndex = highestPriorityIndex;
            int executionTime = tab2[currentProcessIndex].reste;

            currentTime += executionTime;
            tab2[currentProcessIndex].reste = 0;
            tab2[currentProcessIndex].tempsfin = currentTime;

            // Print details of the executed process
            printf("%s\t%d\t\t%d\t\t%d\t\t%d\t\t", tab2[currentProcessIndex].id, tab2[currentProcessIndex].arrive_time, tab2[currentProcessIndex].burst, tab2[currentProcessIndex].priority, tab2[currentProcessIndex].tempsfin);
            printf("\n");

            // Record the executed process index
            output[outputIndex++] = currentProcessIndex;
        }
    }

    printf("\n");

    // Print the Gantt chart based on the execution sequence
    printf("Gantt Chart:\n");
    printf("---------------------------------------------\n");
    printf("|");
    for (int i = 0; i < outputIndex; i++) {
        printf("  %s  |", tab2[output[i]].id);
    }
    printf("\n---------------------------------------------\n");
    

    // Call the function to display Gantt chart in GTK window
    display_prNonp_interface(tab2, output, outputIndex);

   
    return 0;
}

