
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXx 100
#define MAX_QUEUE 3
#define TRUE 1

typedef struct {
    char id[MAXx];
    int arrive_time;
    int burst;
    int priority;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
    int response_time;
    int return_time;
            int execution_time;

    int completed;
    
} Process;

int process_count = 0; // Declare process_count globally

void process_init(Process p[], int len) {
    for (int i = 0; i < len; i++) {
        p[i].waiting_time = 0;
        p[i].remaining_time = 0;
        p[i].turnaround_time = 0;

    }
}

int find_highest_priority(Process p[], int n, int current_time)
{
    int highest_priority = -1;
    int index = -1;

    for (int i = 0; i < n; i++)
    {
        if (!p[i].completed && p[i].arrive_time <= current_time)
        {
            if (highest_priority == -1 || p[i].priority > highest_priority)
            {
                highest_priority = p[i].priority;
                index = i;
            }
        }
    }

    return index;
}


void print_table(Process process[], int n1) {
    puts("+-----+-------------+------------+--------------+-----------------+---------+");
    puts("| PID | Arrive time | Burst Time | Waiting Time | Turnaround Time | Priority|");
    puts("+-----+-------------+------------+--------------+-----------------+---------+");

    for (int i = 0; i < n1; i++) {
        printf("| %2s  |     %2d      |     %2d     |      %2d      |        %2d       |    %2d   |\n",
               process[i].id, process[i].arrive_time, process[i].execution_time, process[i].waiting_time,
               process[i].turnaround_time, process[i].priority);
        puts("+-----+-------------+------------+--------------+-----------------+---------+");
    }
}


void MLFQ(Process process[], int n, int quantum) {
        int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int completed_processes = 0;


    printf("\n---Gantt Chart:\n");
    while (completed_processes < n)
    {
        int selected_process = find_highest_priority(process, n, current_time);

        if (selected_process == -1)
        {
            current_time++;
            continue;
        }

        printf("| %s ", process[selected_process].id);

        if (process[selected_process].response_time == 0)
        {
            process[selected_process].response_time = current_time - process[selected_process].arrive_time;
        }

        process[selected_process].waiting_time += current_time - process[selected_process].return_time;
        process[selected_process].return_time = current_time + 1;
        process[selected_process].burst--;

        if (process[selected_process].burst == 0)
        {
            process[selected_process].completed = TRUE;
            process[selected_process].turnaround_time = current_time - process[selected_process].arrive_time + 1;
            total_waiting_time += process[selected_process].waiting_time;
            total_turnaround_time += process[selected_process].turnaround_time;
            completed_processes++;
        }

        current_time++;
        
        
        /***************RR if same priority********************/
    }
    printf("\n\n");
    
            printf("\nProcess Table with Priority:\n");
    print_table(process, n);
         puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", total_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)total_waiting_time / (double)n);
    printf("Total Turnaround Time   : %-2d\n", total_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)total_turnaround_time / (double)n);


}

int main(int argc, char *argv[]) {
int i;
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    int process_count = 0;
    Process *process;

    FILE *fp = fopen("inputs.txt", "r");
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
    i++;
}
 

    fclose(fp);
    
        printf("Number of processes = %d\n", process_count);
        printf("\n\n");


    int quantum = 0;
    printf("Enter quantum value for process with same priority: ");
    scanf("%d", &quantum);
    

    process_init(process, process_count);
    MLFQ(process, process_count, quantum);


    


    // Free allocated memory
    free(process);

    return 0;
}

    
