#include <../graphique/ProcessesInterface.h>

#include "prioriteNonPreemptive.h"

int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments
    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the number of processes from the file
    int n;
    fscanf(fp, "%d", &n);

    // Create an array to store information about each process
    Process tab2[n];

    // Read process information from the file and populate the array
    for (int i = 0; i < n; i++) {
        int arrive_time, burst, priority;
        char id[ID_LEN];

        // Validate and read process details
        if (fscanf(fp, "%s %d %d %d", id, &arrive_time, &burst, &priority) != 4) {
            printf("Invalid input format.\n");
            return 1;
        }

        // Populate the process structure
        strcpy(tab2[i].id, id);
        tab2[i].arrive_time = arrive_time;
        tab2[i].burst = burst;
        tab2[i].priority = priority;
        tab2[i].reste = burst;
    }

    // Close the file
    fclose(fp);

    // Perform Priority Non-Preemptive Scheduling
    PrioritéNonP(tab2, n);

    // Display process statistics
    displayProcessStats_pr(tab2, n);

    // Display the Gantt chart
    Gantt_Chart(tab2, n);

    // Call the function to display Gantt chart in GTK window
    display_prNonp_interface(tab2, output, outputIndex);

    return 0;
}


