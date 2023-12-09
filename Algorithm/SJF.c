#include "SJF.h"

// Function to implement Shortest Job First (SJF)
void SJF(Process *process, int np) {
    // Sort processes based on burst time using qsort
    qsort(process, np, sizeof(Process), compareBurstTime);

    // Calculate finishing time, waiting time, and turnaround time
    int currentTime = 0;
    for (int i = 0; i < np; i++) {
        // Calculate finishing time
        process[i].tempsfin = currentTime + process[i].burst;

        // Calculate waiting time and turnaround time
        process[i].waitingTime = currentTime - process[i].arrive_time;
        process[i].turnaroundTime = process[i].waitingTime + process[i].burst;

        currentTime = process[i].tempsfin; // Update current time for the next iteration
    }
}





int main(int argc, char *argv[]) {
    int np = 0;

	if (argc != 2) {
	    printf("Usage: %s input_file\n", argv[0]);
	    return 1;
	}

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
	    perror("Error opening file");
	    return 1;
	}

	char line[500];
	while (fgets(line, sizeof(line), fp)) {
	    if (np >= MAX_PROCESSES) {
		printf("Too many processes!\n");
		break;
	    }

        int arrive_time, burst;
        char id[ID_LEN];

        if (sscanf(line, "%s %d %d", id, &arrive_time, &burst) == 3) {
            strcpy(process[np].id, id);
            process[np].arrive_time = arrive_time;
            process[np].burst = burst;

		parseProcessColor(process, np);


            np++;
        }
    }

    fclose(fp);
    
    // Display process information in the console
    SJF(process, np);
    
     displayGanttChart(process,np);
    
     displayProcessTable(process,np); 


	initializeGUI_SJF(argc, argv, process, np);

    return 0;
}

