#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define ID_LEN 20

typedef struct _process {
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

#define MAX_PROCESSES 100 // Define the maximum number of processes

Process process[MAX_PROCESSES]; // Array of Process structs


void displayGanttChartGTK(Process tab2[], int output[], int outputIndex) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *tree_view;
    GtkWidget *box;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Gantt Chart - Round Robin");
    gtk_widget_set_size_request(window, 600, 300);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    // Create a box to hold the header label and tree view
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(scrolled_window), box);

    // Create and configure the header label
    GtkWidget *header_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(header_label), "<span foreground='purple'><big><b>Gantt Chart - Round Robin</b></big></span>");
    gtk_box_pack_start(GTK_BOX(box), header_label, FALSE, FALSE, 0);

    // Create a list store to hold the data to be displayed in the tree view
    GtkListStore *list_store;
    list_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

    // Fill the list store with process information
    for (int i = 0; i < outputIndex; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(list_store, &iter);
        
       

        gtk_list_store_set(list_store, &iter,
                           0, tab2[output[i] - 1].id,
                           1, tab2[output[i] - 1].arrive_time,
                           2, tab2[output[i] - 1].burst,
                           3, tab2[output[i] - 1].return_time,
                           -1);
    }

    // Create the tree view with columns for each piece of information
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    gtk_box_pack_start(GTK_BOX(box), tree_view, TRUE, TRUE, 0);

    // Add columns to the tree view with titles (header names)
    const char *column_names[] = {"Process", "Arrival Time", "Execution Time", "Completion Time"};
    for (int i = 0; i < 4; i++) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(column_names[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    }
    
    gtk_widget_show_all(window);

    gtk_main();
}





int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    Process tab2[50];
    int np = 0;

    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        if (np >= 50) {
            printf("Too many processes!\n");
            break;
        }
        
        int arrive_time, burst, priority;
        char id[ID_LEN];

        if (sscanf(line, "%s %d %d %d", id, &arrive_time, &burst, &priority) == 4) {
            strcpy(tab2[np].id, id);
            tab2[np].arrive_time = arrive_time;
            tab2[np].burst = burst;
            tab2[np].priority = priority;
            tab2[np].completed = 0;
            np++;
        }
    }

    fclose(fp);

    int time_quantum;
    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    printf(" **** Process Scheduling ****\n");
    printf("Process\tArrival Time\tExecution Time\tCompletion Time\n");

    int time = 0;
    int num_processes = np;
    int completed = 0;
    int outputIndex = 0;
    int output[100];

    while (completed < num_processes) {
        for (int i = 0; i < np; i++) {
            if (!tab2[i].completed) {
                if (tab2[i].burst > 0) {
                    if (tab2[i].burst > time_quantum) {
                        printf("%s\t%d\t\t%d\t\t", tab2[i].id, tab2[i].arrive_time, tab2[i].burst);
                        printf("%d (Quantum expired)\n", time + time_quantum);
                        output[outputIndex++] = i + 1;
                        tab2[i].burst -= time_quantum;
                        time += time_quantum;
                    } else {
                        printf("%s\t%d\t\t%d\t\t", tab2[i].id, tab2[i].arrive_time, tab2[i].burst);
                        time += tab2[i].burst;
                        tab2[i].burst = 0;
                        completed++;
                        tab2[i].return_time = time;
                        printf("%d (Completed)\n", time);
                        output[outputIndex++] = i + 1;
                        tab2[i].completed = 1;
                    }
                }
            }
        }
    }

    printf("Order of Execution: ");
    printf("\n---------- ------  ------  ------  ------  ------  ------  ------ --\n");
    for (int i = 0; i < outputIndex; i++) {
        printf("|    p%d   ", output[i]);
    }
    printf("|\n---------- ------  ------  ------  ------  ------  ------  ------ --\n");
    
    
        // Call the function to display Gantt chart in GTK window
    displayGanttChartGTK(tab2, output, outputIndex);

    return 0;
}

