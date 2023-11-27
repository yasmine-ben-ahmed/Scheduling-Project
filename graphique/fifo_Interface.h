//fifo output function in interface using gtk

#ifndef FIFO_H
#define FIFO_H

#include <gtk/gtk.h>

#define ID_LEN 20

// Define the Process type
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

// Global GTK Widgets
GtkWidget *window;
GtkWidget *text_view;
GtkWidget *grid;


void setup_gui(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Process Information - FIFO");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);

    GtkWidget *header_label = gtk_label_new("Process Information");
    gtk_label_set_markup(GTK_LABEL(header_label), "<span foreground='purple'><big><b>First In First Out Algorithm (FIFO) </b></big></span>");

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), text_view, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), header_label, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
}


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

#endif // FIFO_H
