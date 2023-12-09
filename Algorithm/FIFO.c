#include "FIFO.h"
#include "SJF.h"



// Function to implement Shortest Job First (SJF)
void FIFO(Process *process, int np) {
    // Sort processes based on burst time using qsort
    qsort(process, np, sizeof(Process), compareArrivalTime);

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
            process[np].start_time = -1;
            process[np].finish_time = -1;

		parseProcessColor(process, np);

            np++;
        }
        
        
    }

    fclose(fp);

    qsort(process, np, sizeof(Process), compareArrivalTime);

    // Assign start and finish times
    int current_time = 0;
    for (int i = 0; i < np; i++) {
        process[i].start_time = current_time;
        process[i].finish_time = current_time + process[i].burst;
        current_time = process[i].finish_time;
    }

    // Display process information in the console
    FIFO(process, np);
    
     displayGanttChart(process,np);
    
     displayProcessTable(process,np); 
     
initializeGUI_FIFO(argc, argv, process, np);

    return 0;
}

