#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define ID_LEN 20
#define TRUE 1
#define FALSE 0

typedef struct _process
{
    char id[ID_LEN];
    int arrive_time;
    int waiting_time;
    int return_time;
    int turnaround_time;
    int response_time;
    int burst;
    int priority;
    int completed;
} Process;

void process_init(Process p[], int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        p[i].waiting_time = 0;
        p[i].return_time = 0;
        p[i].response_time = 0;
        p[i].completed = FALSE;
    }
}

void print_table(Process p[], int k);
void print_gantt_chart(Process p[], int k);

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

int main(int argc, char *argv[])
{
    int i, j;
    if (argc < 2)
    {
        printf("Not enough arguments!!!\n");
        return 1;
    }

    if (argc > 2)
    {
        printf("Too many arguments!!!\n");
        return 1;
    }

    if (argv[1] == NULL)
    {
        printf("Invalid Argument!!!\n");
        return 1;
    }

    printf("Argc = %d\n", argc);

    for (j = 0; j < argc; ++j)
    {
        printf("Argv[%d] = %s\n", j, argv[j]);
    }

    printf("C Program for Priority Scheduling with Preemption\n");
    int process_count = 0;

    Process *process;
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("FILE OPEN ERROR!\n");
        return 1;
    }

    fscanf(fp, " %d", &process_count);
    process = (Process *)malloc(sizeof(Process) * process_count);

    while (i < process_count)
    {
        fscanf(fp, "%s %d %d %d", process[i].id, &process[i].arrive_time, &process[i].burst, &process[i].priority);
        i++;
    }
    fclose(fp);

    int k = i; // determine the number of processes in the text file

    printf("Number of processes = %d\n", k);

    process_init(process, k);

    int current_time = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int completed_processes = 0;

    printf("\nGantt Chart:\n");
    while (completed_processes < k)
    {
        int selected_process = find_highest_priority(process, k, current_time);

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
    }

    // Print table
    puts(""); // Empty line
    print_table(process, k);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", total_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)total_waiting_time / (double)k);
    printf("Total Turnaround Time   : %-2d\n", total_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)total_turnaround_time / (double)k);

    free(process);
    return 0;
}

void print_table(Process process[], int n1)
{
    int i;
    int n2 = n1;
    puts("+-----+------------+--------------+-----------------+");
    puts("| PID | Burst Time | Waiting Time | Turnaround Time |");
    puts("+-----+------------+--------------+-----------------+");

    for (i = 0; i < n1; i++)
    {
        printf("| %2s  |     %2d     |      %2d      |        %2d       |\n", process[i].id, process[i].burst, process[i].waiting_time, process[i].turnaround_time);
        puts("+-----+------------+--------------+-----------------+");
    }
}

void print_gantt_chart(Process process[], int n)
{
    int i, j, n2;
    n2 = n;
    // print top bar
    printf(" ");
    for (i = 0; i < n2; i++)
    {
        for (j = 0; j < process[i].turnaround_time; j++)
            printf("--");
        printf(" ");
    }
    printf("\n|");

    // printing process id in the middle
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < process[i].turnaround_time - 1; j++)
            printf(" ");
        printf("%s", process[i].id);
        for (j = 0; j < process[i].turnaround_time - 1; j++)
            printf(" ");
        printf("|");
    }
    printf("\n ");
    // printing bottom bar
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < process[i].turnaround_time; j++)
            printf("--");
        printf(" ");
    }
    printf("\n");

    // printing the time line
    printf("0");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < process[i].turnaround_time; j++)
            printf("  ");
        if (process[i].turnaround_time > 9)
            printf("\b"); // backspace : remove 1 space
        printf("%d", process[i].turnaround_time);
    }
    printf("\n");

    // print the time axis
    printf(" ");
    for (i = 0; i < n2; i++)
    {
        for (j = 0; j < process[i].turnaround_time; j++)
            printf("  ");
        printf("%d", process[i].turnaround_time);
    }
    printf("\n");
}
