#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#include <../graphique/RR_Inter.h>

#include <../graphique/ProcessesInterface.h>


#define MAX_PROCESSES 100 // Define the maximum number of processes

Process process[MAX_PROCESSES]; // Array of Process structs






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

    int time_quantum;
    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    printf(" **** Process Scheduling ****\n");
    printf("Process\tArrival Time\tExecution Time\tCompletion Time\n");

    int time = 0;
    int num_processes = np;
    int completed = 0;
    int outputIndex = 0;
    int output[100];

    while (completed < num_processes) {
        for (int i = 0; i < np; i++) {
            if (!tab2[i].completed) {
                if (tab2[i].burst > 0) {
                    if (tab2[i].burst > time_quantum) {
                        printf("%s\t%d\t\t%d\t\t", tab2[i].id, tab2[i].arrive_time, tab2[i].burst);
                        printf("%d (Quantum expired)\n", time + time_quantum);
                        output[outputIndex++] = i + 1;
                        tab2[i].burst -= time_quantum;
                        time += time_quantum;
                    } else {
                        printf("%s\t%d\t\t%d\t\t", tab2[i].id, tab2[i].arrive_time, tab2[i].burst);
                        time += tab2[i].burst;
                        tab2[i].burst = 0;
                        completed++;
                        tab2[i].return_time = time;
                        printf("%d (Completed)\n", time);
                        output[outputIndex++] = i + 1;
                        tab2[i].completed = 1;
                    }
                }
            }
        }
    }

    printf("Order of Execution: ");
    printf("\n---------- ------  ------  ------  ------  ------  ------  ------ --\n");
    for (int i = 0; i < outputIndex; i++) {
        printf("|    p%d   ", output[i]);
    }
    printf("|\n---------- ------  ------  ------  ------  ------  ------  ------ --\n");
    
    
        // Call the function to display Gantt chart in GTK window
    display_roundrobin_interface(tab2, output, outputIndex);

    return 0;
}

