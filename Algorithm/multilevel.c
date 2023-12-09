#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXx 100
#define MAX_QUEUE 3
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 10

typedef struct {
    char id[MAXx];
    int arrive_time;
    int burst;
    int priority;
    int turnaround_time;
} Process;

typedef struct {
    Process queue[MAX_SIZE];
    int front, rear;
} Queue;

Queue queues[MAX_QUEUE];

// Function to initialize all the queues
void initializeQueues() {
    for (int i = 0; i < MAX_QUEUE; i++) {
        queues[i].front = -1;
        queues[i].rear = -1;
    }
}

// Function to add a process to a specific priority queue
void enqueue(Process p, int priority) {
    if (queues[priority].rear == MAX_SIZE - 1) {
        printf("Queue Overflow\n");
        return;
    }

    if (queues[priority].front == -1) {
        queues[priority].front = 0;
    }

    queues[priority].rear++;
    queues[priority].queue[queues[priority].rear] = p;

    printf("Enqueued process %s at priority %d\n", p.id, priority); // Debug print
}


// Function to remove a process from a specific priority queue
Process dequeue(int priority) {
    Process empty;
    strcpy(empty.id, "");
    empty.burst = -1;

    if (queues[priority].front == -1 || queues[priority].front > queues[priority].rear) {
        printf("Queue Underflow\n");
        return empty;
    }

    Process dequeued = queues[priority].queue[queues[priority].front];
    queues[priority].front++;
    return dequeued;
}

// Function implementing Multilevel Feedback Queue scheduling algorithm
void MLFQ(Process process[], int n, int quantum) {
    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int completed_processes = 0;

while (completed_processes < n) {
    int flag = FALSE;

    for (int priority = 0; priority < MAX_QUEUE; priority++) {
        if (queues[priority].front != -1) {
            Process selected_process = dequeue(priority);

            printf("| %s ", selected_process.id);

            if (strcmp(selected_process.id, "") == 0) {
                continue;
            }

            flag = TRUE;

            if (selected_process.burst > quantum) {
                current_time += quantum;
                selected_process.burst -= quantum;

                while (completed_processes < n && process[completed_processes].arrive_time <= current_time) {
                    enqueue(process[completed_processes], process[completed_processes].priority);
                    completed_processes++;
                }

                enqueue(selected_process, priority);
            } else {
                current_time += selected_process.burst;
                selected_process.turnaround_time = current_time - selected_process.arrive_time;
                total_waiting_time += selected_process.turnaround_time - selected_process.burst;
                total_turnaround_time += selected_process.turnaround_time;
                selected_process.burst = 0;
                completed_processes++;
            }
        }
    }

    if (!flag) {
        current_time++;
    }

    // Debug print to check queue state
    printf("Time: %d, Current Process Queue State: ", current_time);
    for (int q = 0; q < MAX_QUEUE; q++) {
        printf("Queue %d: Front = %d, Rear = %d | ", q, queues[q].front, queues[q].rear);
    }
    printf("\n");
}
	



    printf("\n\nTotal Waiting Time      : %-2d\n", total_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)total_waiting_time / (double)n);
    printf("Total Turnaround Time   : %-2d\n", total_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)total_turnaround_time / (double)n);
}

int main(int argc, char *argv[]) {
    int i = 0;

    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    int process_count = 0;
    Process *processes;

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("FILE OPEN ERROR!\n");
        return 1;
    }

    fscanf(fp, " %d", &process_count);
    processes = (Process *)malloc(sizeof(Process) * process_count);

    while (i < process_count) {
        fscanf(fp, "%s %d %d %d", processes[i].id, &processes[i].arrive_time, &processes[i].burst, &processes[i].priority);

        int execution_time = processes[i].burst;
        processes[i].burst = execution_time;
        i++;
    }

    fclose(fp);

    printf("Number of processes = %d\n\n", process_count);

    int quantum = 0;
    printf("Enter quantum value for all priority queues: ");
    scanf("%d", &quantum);

    initializeQueues();
    MLFQ(processes, process_count, quantum);

    free(processes);

    return 0;
}

