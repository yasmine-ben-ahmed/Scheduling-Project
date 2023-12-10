#include "FIFO.h" // Include header file for FIFO algorithm

int main(int argc, char *argv[]) {
    int np = 0; // Number of processes

    // Check if the correct number of arguments is provided
    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r"); // Open file for reading
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

        // Extract process information from the file
        if (sscanf(line, "%s %d %d", id, &arrive_time, &burst) == 3) {
            strcpy(process[np].id, id);
            process[np].arrive_time = arrive_time;
            process[np].burst = burst;
            process[np].start_time = -1;
            process[np].finish_time = -1;

            parseProcessColor(process, np); // Parse process color for visualization

            np++;
        }
    }

    fclose(fp); // Close the file

    qsort(process, np, sizeof(Process), compareArrivalTime); // Sort processes by arrival time

    // Assign start and finish times to processes
    int current_time = 0;
    for (int i = 0; i < np; i++) {
        process[i].start_time = current_time;
        process[i].finish_time = current_time + process[i].burst;
        current_time = process[i].finish_time;
    }

    // Execute FIFO algorithm
    FIFO(process, np);

    // Display Gantt chart for visualization
    displayGanttChart(process, np);

    // Display process table
    displayProcessTable(process, np);

    // Initialize FIFO algorithm GUI (if applicable)
    initializeGUI_FIFO(argc, argv, process, np);

    return 0;
}

