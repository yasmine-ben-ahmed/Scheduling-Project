void print_table(Process p[], int k);
int find_highest_priority(Process p[], int n, int current_time);
void process_init(Process p[], int len);

void process_init(Process p[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        p[i].waiting_time = 0;
        p[i].return_time = 0;
        p[i].response_time = 0;
        p[i].completed = FALSE;
    }
}

void print_table(Process process[], int n1) {
    int i;
    int n2 = n1;
    puts("+-----+--------------+------------+---------+--------------+-----------------+------------------+");
    puts("| PID | Arrival Time | Burst Time | Priority| Waiting Time | Turnaround Time | Completion Time |");
    puts("+-----+--------------+------------+---------+--------------+-----------------+------------------+");

    for (i = 0; i < n1; i++) {
        printf("| %2s  |      %2d      |     %2d     |    %2d   |      %2d      |        %2d       |        %2d        |\n",
               process[i].id, process[i].arrive_time, process[i].execution_time, process[i].priority,
               process[i].waiting_time, process[i].turnaround_time, process[i].return_time);
        puts("+-----+--------------+------------+---------+--------------+-----------------+------------------+");
    }
}

int find_highest_priority(Process p[], int n, int current_time) {
    int highest_priority = -1;
    int index = -1;

    for (int i = 0; i < n; i++) {
        if (!p[i].completed && p[i].arrive_time <= current_time) {
            if (highest_priority == -1 || p[i].priority > highest_priority) {
                highest_priority = p[i].priority;
                index = i;
            }
        }
    }

    return index;
}

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
        output[outputIndex++] = selected_process + 1;

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

    puts("");
    print_table(process, process_count);

    puts("");
    printf("Total Waiting Time      : %-2d\n", total_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)total_waiting_time / (double)process_count);
    printf("Total Turnaround Time   : %-2d\n", total_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)total_turnaround_time / (double)process_count);
}

