#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define ID_LEN 20

typedef struct {
    char id[ID_LEN];
    int nbrp;
    int num;
    int ta;
    int te;
    int pr;
    int reste;
    int tempsfin;
    int waitingTime;
} Info;


void displayGanttChartGTK(Info tab2[], int output[], int outputIndex) {
    GtkWidget *window;
    GtkWidget *scrolled_window;
    GtkWidget *tree_view;
    GtkWidget *box; 
    
    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Process Information");
    gtk_widget_set_size_request(window, 600, 300);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    // Create a box to hold both header label and tree view
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(scrolled_window), box);

    // Create and configure the header label
    GtkWidget *header_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(header_label), "<span foreground='purple'><big><b>Priorite Non Preemptive Algorithm </b></big></span>");
    gtk_box_pack_start(GTK_BOX(box), header_label, FALSE, FALSE, 0);
    

    // Create a list store to hold the data to be displayed in the tree view
    GtkListStore *list_store;
    list_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

    // Fill the list store with process information
    for (int i = 0; i < outputIndex; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(list_store, &iter);

        gtk_list_store_set(list_store, &iter,
                           0, tab2[output[i]].id,
                           1, tab2[output[i]].ta,
                           2, tab2[output[i]].te,
                           3, tab2[output[i]].pr,
                           4, tab2[output[i]].tempsfin,
                           -1);
    }

    // Create the tree view with columns for each piece of information
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    gtk_box_pack_start(GTK_BOX(box), tree_view, TRUE, TRUE, 0);

    // Add columns to the tree view with titles (header names)
    const char *column_names[] = {"Process", "Arrival Time", "Execution Time", "Priority", "Completion Time"};
    for (int i = 0; i < 5; i++) {
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

    int n;
    fscanf(fp, "%d", &n); // Read the number of processes

    Info tab2[n]; // Assuming the number of processes is provided in the file

    for (int i = 0; i < n; i++) {
        int ta, te, pr;
        char id[ID_LEN];

        if (fscanf(fp, "%s %d %d %d", id, &ta, &te, &pr) != 4) {
            printf("Invalid input format.\n");
            return 1;
        }

        strcpy(tab2[i].id, id);
        tab2[i].ta = ta;
        tab2[i].te = te;
        tab2[i].pr = pr;
        tab2[i].num = i + 1;
        tab2[i].waitingTime = 0;
        tab2[i].reste = te;
    }

    fclose(fp);

    int currentTime = tab2[0].ta; // Set the initial time to the arrival time of the first process

    printf(" **** Priority Process Scheduling ****\n");
    printf("Process\tArrival Time\tExecution Time\tPriority\tCompletion Time\n");

    int output[n]; // Record the sequence of executed processes
    int outputIndex = 0; // Index for output array

    while (1) {
        int highestPriorityIndex = -1;
        int highestPriority = -1; // Initialize with lowest possible priority

        // Find the highest priority process that has arrived and not yet completed
        for (int j = 0; j < n; j++) {
            if (tab2[j].reste > 0 && tab2[j].ta <= currentTime && tab2[j].pr > highestPriority) {
                highestPriority = tab2[j].pr;
                highestPriorityIndex = j;
            }
        }

        if (highestPriorityIndex == -1) {
            int nextProcessTime = 9999; // Set a large value as default

            // Find the next process arrival time
            for (int j = 0; j < n; j++) {
                if (tab2[j].reste > 0 && tab2[j].ta > currentTime && tab2[j].ta < nextProcessTime) {
                    nextProcessTime = tab2[j].ta;
                }
            }

            if (nextProcessTime == 9999) {
                // All processes are completed
                break;
            } else {
                currentTime = nextProcessTime;
            }
        } else {
            int currentProcessIndex = highestPriorityIndex;
            int executionTime = tab2[currentProcessIndex].reste;

            currentTime += executionTime;
            tab2[currentProcessIndex].reste = 0;
            tab2[currentProcessIndex].tempsfin = currentTime;

            // Print details of the executed process
            printf("%s\t%d\t\t%d\t\t%d\t\t%d\t\t", tab2[currentProcessIndex].id, tab2[currentProcessIndex].ta, tab2[currentProcessIndex].te, tab2[currentProcessIndex].pr, tab2[currentProcessIndex].tempsfin);
            printf("\n");

            // Record the executed process index
            output[outputIndex++] = currentProcessIndex;
        }
    }

    printf("\n");

    // Print the Gantt chart based on the execution sequence
    printf("Gantt Chart:\n");
    printf("---------------------------------------------\n");
    printf("|");
    for (int i = 0; i < outputIndex; i++) {
        printf("  %s  |", tab2[output[i]].id);
    }
    printf("\n---------------------------------------------\n");
    

    // Call the function to display Gantt chart in GTK window
    displayGanttChartGTK(tab2, output, outputIndex);

   
    return 0;
}

