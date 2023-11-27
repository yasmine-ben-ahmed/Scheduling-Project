#include<stdio.h>
#include<sched.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>


#include <../graphique/fifo_Interface.h>


#define MAXx 100

#define TRUEe 1
#define FALSEe 0


// Function Prototypes
void process_init(Process p[], int len);
void print_table(Process p[], int k);
void print_gantt_chart(Process p[], int k);
//void display_process_information();



/**-------------------------------------------------------------------------*/
void process_init(Process p[], int len)
{
    int i;

    
    for (i = 0; i < len; i++)
    {
        p[i].waiting_time = 0;   
        p[i].return_time = 0;    
        p[i].response_time = 0;  
        p[i].completed = FALSEe;  
    }
}


/**-------------------------------------------------------------------------*/
void print_table(Process process[], int n1)
{
    int i;
    //int n2=n1;
    puts("+-----+------------+--------------+-----------------+");
    puts("| PID | Burst Time | Waiting Time | Turnaround Time |");
    puts("+-----+------------+--------------+-----------------+");

    for(i=0; i<n1; i++) {
        printf("| %2d  |     %2d     |      %2d      |        %2d       |\n",i+1,process[i].burst, process[i].waiting_time,process[i].turnaround_time );
        puts("+-----+------------+--------------+-----------------+");
    }

}



/**-------------------------------------------------------------------------*/


void print_gantt_chart(Process process[], int n)
{
    int i, j,n2;
    n2=n;
    // print top bar
    printf(" ");
    for(i=0; i<n2; i++) {
        for(j=0; j<process[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n|");

    // printing process id in the middle
    for(i=0; i<n; i++) {
        for(j=0; j<process[i].burst - 1; j++) printf(" ");
        printf("p%d", i+1);
        for(j=0; j<process[i].burst - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // printing bottom bar
    for(i=0; i<n; i++) {
        for(j=0; j<process[i].burst; j++) printf("--");
        printf(" ");
    }
    printf("\n");

    // printing the time line
    printf("0");
    for(i=0; i<n; i++) {
        for(j=0; j<process[i].burst; j++) printf("  ");
        if(process[i].turnaround_time > 9) printf("\b"); // backspace : remove 1 space
        printf("%d", process[i].turnaround_time);

    }
    printf("\n");

}



int main(int argc, char *argv[])
{

setup_gui(argc, argv);


    int bt[20], wt[20], tat[20], i, n, total = 0, pos, temp;
    float avg_wt, avg_tat;
    //Process p[MAXx];
    int sum_waiting_time = 0, sum_turnaround_time;
    int k = 0;
    int j;

    if (argc < 2)
    {
        printf("Not enough arguments!!!\n");
        return 1;
    }

    if (argc > 2)
    {
        printf("Too many argumnts!!!\n");
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

    printf("C Program for First Come First Serve (FCFS) Scheduling Algorithm\n");
    int process_count = 0;

    //Quantum quantum;
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
    k = i; //determnier nombre processus dans fichier texte
    ////////////////////////////////
    printf("nombre de processus=%d\n", k);
    n = k;
    printf("NBR=%d\n", n);
    printf("temps d'arrive\n");
    for (i = 0; i < n; i++)
    {
        printf("DA[%d]=%d\n", i + 1, process[i].arrive_time);
    }
    printf("temps d'exuction(burst time):\n");
    for (i = 0; i < n; i++)
    {
        printf("\nP[%d]=%d ", i + 1, process[i].burst);
        process[i].waiting_time = process[i].turnaround_time = 0;
    }
    //////////////////////

    // calculate waiting time and turnaround time
    process[0].turnaround_time = process[0].burst;
    for (i = 1; i < n; i++)
    {
        process[i].waiting_time = process[i - 1].waiting_time + process[i - 1].burst;
        process[i].turnaround_time = process[i].waiting_time + process[i].burst;
    }

    // calculate sum of waiting time and sum of turnaround time
    for (i = 0; i < n; i++)
    {
        sum_waiting_time += process[i].waiting_time;
        sum_turnaround_time += process[i].turnaround_time;
    }

    // Set a single property to store 'process' and 'process_count'
    g_object_set_data(G_OBJECT(text_view), "process-data", process);
    g_object_set_data(G_OBJECT(text_view), "process-count", GINT_TO_POINTER(process_count));

    // Directly call the display_process_information function
    display_process_information(text_view, NULL);

    // print table
    puts(""); // Empty line
    print_table(process, n);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double)k);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double)k);

    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          ***********          ");
    print_gantt_chart(process, n);

    gtk_main();
    return 0;
}





