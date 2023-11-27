#include<stdio.h>
#include<sched.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#include <gtk/gtk.h>


#define MAXx 100

#define ID_LEN 20
#define TRUEe 1
#define FALSEe 0


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


// Function Prototypes
void process_init(Process p[], int len);
void print_table(Process p[], int k);
void print_gantt_chart(Process p[], int k);
void display_process_information();


// Global GTK Widgets
GtkWidget *window;
GtkWidget *text_view;
GtkWidget *grid;

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


/**-------------------------------------------------------------------------*/

void display_process_information(GtkWidget *widget, gpointer data) {

    Process *process = (Process *)g_object_get_data(G_OBJECT(widget), "process-data");
    int process_count = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "process-count"));

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, "Process Information:\n\n", -1);
    
	    
	// Clear the text buffer
	gtk_text_buffer_set_text(buffer, "", -1);

	// Apply colors and formatting to the header
	const gchar *header_markup = "<span foreground='white'><b>PID</b></span> | <span foreground='white'><b>Burst Time</b></span> | <span foreground='white'><b>Waiting Time</b></span> | <span foreground='white'><b>Turnaround Time</b></span>\n";

	GtkTextIter iter;
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_markup(buffer, &iter, header_markup, -1);

	// Create a separator line after the header for better readability
	const gchar *separator_markup = "-----------------------------------------------------------------------------------------\n";
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_markup(buffer, &iter, separator_markup, -1);

	// Print process information row by row in a table format with colors
	for (int i = 0; i < process_count; i++) {
	    gchar *row_info_markup = g_strdup_printf("<span foreground='white'>| %-5d | %-25d | %-30d | %-30d |</span>\n",
		                                   i + 1, process[i].burst, process[i].waiting_time, process[i].turnaround_time);
	    gtk_text_buffer_get_end_iter(buffer, &iter);
	    gtk_text_buffer_insert_markup(buffer, &iter, row_info_markup, -1);
	    g_free(row_info_markup);
	}
    gtk_text_buffer_insert_at_cursor(buffer, "\n-----------------------------------------------------------------------------------------\n", -1);




    // Print Gantt chart
    char gantt_chart[500]; // Adjust size as needed
    sprintf(gantt_chart, "\nGantt Chart:\n");

    int max_time = 0;
    for (int i = 0; i < process_count; i++) {
        max_time = (process[i].turnaround_time > max_time) ? process[i].turnaround_time : max_time;
    }
    for (int i = 0; i < process_count; i++) {
        sprintf(gantt_chart + strlen(gantt_chart), "p%-2d", i + 1);
        for (int j = 0; j < process[i].burst; j++) {
            sprintf(gantt_chart + strlen(gantt_chart), "______.");
        }
        sprintf(gantt_chart + strlen(gantt_chart), "|");
    }
    gtk_text_buffer_insert_at_cursor(buffer, gantt_chart, -1);
    
    gtk_text_buffer_insert_at_cursor(buffer, "\n------------------------------------------------------------------------------------------------------------------------------\n", -1);
    

    // Print timeline
    char timeline[1000]; 
    sprintf(timeline, "\nTimeline:\n");
    sprintf(timeline + strlen(timeline), "0 ");
    for (int t = 1; t <= max_time; t++) {
        sprintf(timeline + strlen(timeline), "%-2d", t);
        sprintf(timeline + strlen(timeline), "______");
    }
    gtk_text_buffer_insert_at_cursor(buffer, timeline, -1); 
    
    
gtk_text_buffer_insert_at_cursor(buffer, "-----------------------------------------------------------------------------------------\n", -1);
    
    // Calculate waiting and turnaround times
    int sum_waiting_time = 0, sum_turnaround_time = 0;
    for (int i = 0; i < process_count; i++) {
        sum_waiting_time += process[i].waiting_time;
        sum_turnaround_time += process[i].turnaround_time;
    }


    // Print total and average waiting time
    double avg_waiting_time = (double)sum_waiting_time / (double)process_count;
    char waiting_info[200];
    sprintf(waiting_info, "\nTotal Waiting Time      : %-2d\n", sum_waiting_time);
    gtk_text_buffer_insert_at_cursor(buffer, waiting_info, -1);
    
    
gtk_text_buffer_insert_at_cursor(buffer, "-----------------------------------------------------------------------------------------\n", -1);

    sprintf(waiting_info, "Average Waiting Time    : %-2.2lf\n", avg_waiting_time);
    gtk_text_buffer_insert_at_cursor(buffer, waiting_info, -1);

gtk_text_buffer_insert_at_cursor(buffer, "-----------------------------------------------------------------------------------------\n", -1);
    // Print total and average turnaround time
    double avg_turnaround_time = (double)sum_turnaround_time / (double)process_count;
    char turnaround_info[200];
    sprintf(turnaround_info, "Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    gtk_text_buffer_insert_at_cursor(buffer, turnaround_info, -1);
    
gtk_text_buffer_insert_at_cursor(buffer, "-----------------------------------------------------------------------------------------\n", -1);
    sprintf(turnaround_info, "Average Turnaround Time : %-2.2lf\n", avg_turnaround_time);
    gtk_text_buffer_insert_at_cursor(buffer, turnaround_info, -1);
    
gtk_text_buffer_insert_at_cursor(buffer, "-----------------------------------------------------------------------------------------\n", -1);
}



/**-------------------------------------------------------------------------*/




int main (int argc, char* argv[])
{

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Process Information");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

    GtkWidget *button = gtk_button_new_with_label("Display Process Info");
    
    GtkWidget *header_label = gtk_label_new("Process Information");
gtk_label_set_markup(GTK_LABEL(header_label), "<span foreground='purple'><big><b>First In First Out Algorithm (FIFO) </b></big></span>");
   
GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);


    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    
    gtk_box_pack_start(GTK_BOX(vbox), header_label, FALSE, FALSE, 10);
gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);
gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
    
    	
       int bt[20],wt[20],tat[20],i,n,total=0,pos,temp;
       float avg_wt,avg_tat;
       //Process p[MAXx];
       int sum_waiting_time = 0, sum_turnaround_time;
     int k=0;
      int j;
	
	if (argc < 2) {
		printf("Not enough arguments!!!\n");
		return 1;
	}
	
	if (argc > 2) {
		printf("Too many argumnts!!!\n");
		return 1;
	}
	
	if (argv[1] == NULL) {
		printf("Invalid Argument!!!\n");
		return 1;
	}

	printf("Argc = %d\n", argc);
	
	for (j = 0; j < argc; ++j) {
		printf("Argv[%d] = %s\n", j, argv[j]);
	}
	

printf("C Program for First Come First Serve (FCFS) Scheduling Algorithm\n");
	int process_count = 0;
	
	//Quantum quantum;
	Process *process;
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
        	printf("FILE OPEN ERROR!\n");
        	return 1;
        }
        
        fscanf(fp, " %d", &process_count);
        process = (Process *)malloc(sizeof(Process) * process_count);
        
        while (i < process_count) {
        	fscanf(fp, "%s %d %d %d", process[i].id, &process[i].arrive_time, &process[i].burst, &process[i].priority);
        	i++;
        }
     k=i;//determnier nombre processus dans fichier texte
        ////////////////////////////////
        printf("nombre de processus=%d\n",k);
      n=k;
           printf("NBR=%d\n",n);
        printf("temps d'arrive\n");
        for(i=0;i<n;i++){
        printf("DA[%d]=%d\n",i+1,process[i].arrive_time);
        }
        printf("temps d'exuction(burst time):\n");
        for(i=0;i<n;i++){
       printf("\nP[%d]=%d ", i+1,process[i].burst);
      process[i].waiting_time= process[i].turnaround_time= 0;
       }
       //////////////////////

 

    // calculate waiting time and turnaround time
    process[0].turnaround_time = process[0].burst;
    for(i=1; i<n; i++) {
        process[i].waiting_time = process[i-1].waiting_time + process[i-1].burst;
        process[i].turnaround_time = process[i].waiting_time + process[i].burst;

    }

    // calculate sum of waiting time and sum of turnaround time
    for(i=0; i<n; i++) {
            sum_waiting_time += process[i].waiting_time;
            sum_turnaround_time += process[i].turnaround_time;
    }



    // Set a single property to store 'process' and 'process_count'
    g_object_set_data(G_OBJECT(button), "process-data", process);
    g_object_set_data(G_OBJECT(button), "process-count", GINT_TO_POINTER(process_count));

    // Connect the button click event to a callback function
    g_signal_connect(button, "clicked", G_CALLBACK(display_process_information), NULL);



    // print table
    puts(""); // Empty line
    print_table(process, n);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_waiting_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_waiting_time / (double) k);
    printf("Total Turnaround Time   : %-2d\n", sum_turnaround_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_turnaround_time / (double) k);

    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          ***********          ");
    print_gantt_chart(process, n);
    
    
    
    
   
    gtk_main();
    return 0;
}




