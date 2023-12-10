#include <graphics.h>
#include <../Algorithm/SRT.h>
const char* get_process_color_prprem(int process_number);

/*================================================================*/
/**********________priorité_non_preemptive_Interface________********/
/*================================================================*/

// Function to display the interface for Priority Non-Preemptive scheduling algorithm
void display_prNonp_interface(Process tab2[], int output[], int outputIndex) {

    // Call the function to initialize GTK and create the main window
    initialize_and_create_window(&window, &scrolled_window, &box);

    // Create a label for the Priority Non-Preemptive algorithm
    GtkWidget* pr_label = create_markup_label("<span foreground='purple'><big><b>Priorite Non Preemptive Algorithm </b></big></span>");
    gtk_box_pack_start(GTK_BOX(box), pr_label, FALSE, FALSE, 0);
    
/***********************________Order of execution________********************/    

    // Display order of execution
    display_order_of_execution_pr(box, tab2, output, outputIndex);
    
/***********************________Process Information In Table________********************/  

    // Create a list store to store data for the tree view
    GtkListStore *list_store;
    list_store = gtk_list_store_new(7, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

    // Populate the list store with data from the 'tab2' array
    for (int i = 0; i < outputIndex; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(list_store, &iter);

        // Calculate turnaround time and waiting time
        int turnaroundTime = tab2[output[i]].tempsfin - tab2[output[i]].arrive_time;
        int waitingTime = turnaroundTime - tab2[output[i]].burst;

        // Set data for each column in the list store
        gtk_list_store_set(list_store, &iter,
                           0, tab2[output[i]].id,
                           1, tab2[output[i]].arrive_time,
                           2, tab2[output[i]].burst,
                           3, tab2[output[i]].priority,
                           4, waitingTime,
                           5, turnaroundTime,
                           6, tab2[output[i]].tempsfin,
                           -1);
    }

    // Create a tree view with the list store as the model
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    gtk_box_pack_start(GTK_BOX(box), tree_view, TRUE, TRUE, 0);

    // Define column names for the tree view
    const char *column_names[] = {"Process", "Arrival Time", "Execution Time", "Priority", "Waiting Time", "Turnaround Time", "Completion Time"};

    // Create columns in the tree view and set their attributes
    for (int i = 0; i < 7; i++) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(column_names[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    }
    
/***********************________Calculate and Display Averages ________********************/  

    // Declare variables for the order of execution loop
    char outputt[100]; // Buffer to store formatted output
    int n = outputIndex; // Number of processes
    int totalWaitingTime = 0, totalTurnaroundTime = 0; // Declare variables for total waiting and turnaround times

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        totalWaitingTime += tab2[output[i]].tempsfin - tab2[output[i]].arrive_time - tab2[output[i]].burst;
        totalTurnaroundTime += tab2[output[i]].tempsfin - tab2[output[i]].arrive_time;
    }

    // Display average waiting time label
    display_average_time_label(box, "Average Waiting Time (TAM)", totalWaitingTime, n);

    // Display average turnaround time label
    display_average_time_label(box, "Average Turnaround Time (TRM)", totalTurnaroundTime, n);

    // Show all the widgets in the GTK window
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();
}

/*================================================================*/
/**********________Round-Robin_Interface________********/
/*================================================================*/

// Function to display the interface for Round Robin scheduling algorithm
void display_roundrobin_interface(Process tab2[], int output[], int outputIndex,int np, int time_quantum) {

    // Call the function to initialize GTK and create the main window
    initialize_and_create_window(&window, &scrolled_window, &box);

    // Create a label for the Round Robin algorithm
    GtkWidget* pr_label = create_markup_label("<span foreground='purple'><big><b>Round Robin -RR- Algorithm </b></big></span>");
    gtk_box_pack_start(GTK_BOX(box), pr_label, FALSE, FALSE, 0);
    
/***********************________Order of execution________********************/   

    // Display order of execution
    display_order_of_execution_rr(box, tab2, output, outputIndex);
    
/***********************________Process Information In Table________********************/  

    // Display Process Information Tree View
    GtkListStore *list_store;
    list_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

    // Fill the list store with process information
    for (int i = 0; i < outputIndex; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(list_store, &iter);

        // Calculate turnaround time and waiting time
        int turnaroundTime = tab2[output[i] - 1].completion_time - tab2[output[i] - 1].arrive_time;
        int waitingTime = turnaroundTime - tab2[output[i] - 1].execution_time;

        gtk_list_store_set(list_store, &iter,
                           0, tab2[output[i] - 1].id,
                           1, tab2[output[i] - 1].arrive_time,
                           2, tab2[output[i] - 1].execution_time,
                           //3, tab2[output[i] - 1].completion_time,
                           -1);
    }

    // Create the tree view with columns for each piece of information
    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    gtk_box_pack_start(GTK_BOX(box), tree_view, TRUE, TRUE, 0);

    // Add columns to the tree view with titles (header names)
    const char *column_names[] = {"Process", "Arrival Time", "Execution Time"};
    for (int i = 0; i < 3; i++) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(column_names[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    }
    
    
    /**************/
    // Create a list store to hold the process information
GtkListStore *list_store2;
list_store2 = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT);

// Fill the list store with process information
for (int i = 0; i < np; i++) {
    GtkTreeIter iter2;
    gtk_list_store_append(list_store2, &iter2);
    
        // Calculate turnaround time and waiting time
    int turnaroundTime = tab2[i].completion_time - tab2[i].arrive_time;
    int waitingTime = turnaroundTime - tab2[i].execution_time;
    

    gtk_list_store_set(list_store2, &iter2,
                       0, tab2[i].id,
                       1, waitingTime,
                       2, turnaroundTime,
                       -1);
}

// Create the tree view with columns for each piece of information
GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store2));
gtk_box_pack_start(GTK_BOX(box), tree_view, TRUE, TRUE, 0);

// Add columns to the tree view with titles (header names)
const char *column_names2[] = {"Process", "Waiting Time", "Turnaround Time"};
for (int i = 0; i < 3; i++) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(column_names2[i], renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
}

/***********************________Calculate and Display Averages ________********************/ 

    // Calculate and Display Averages
   char outputt[100]; // Adjust the size as needed
    int n = outputIndex; // Correct the value of n
    int totalWaitingTime = 0, totalTurnaroundTime = 0; // Declare the variables here


	    // Calculate and display waiting and turnaround times for each process
    for (int i = 0; i < np; i++) {
    int turnaroundTime1 = tab2[i].completion_time - tab2[i].arrive_time;
    int waitingTime1 = turnaroundTime1 - tab2[i].execution_time;

        totalWaitingTime += waitingTime1;
        totalTurnaroundTime += turnaroundTime1;

    }
    
    
    // Display average waiting time label
    display_average_time_label(box, "Average Waiting Time (TAM)", totalWaitingTime, np);

    // Display average turnaround time label
    display_average_time_label(box, "Average Turnaround Time (TRM)", totalTurnaroundTime, np);

    // Show all the widgets in the GTK window
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();
}

/*================================================================*/
/**********________SRT_Interface________********/
/*================================================================*/

void display_SRT_interface(Process processes[], int n, int output[], int outputIndex) {
    // Console output of the sequence of processes
   /* printf("Sequence of processes console from srt inter: ");
    for (int i = 0; i < outputIndex; i++) {
        printf("%s ", processes[output[i]].id);
    }
    printf("\n");*/

    // Initialize GTK
    gtk_init(NULL, NULL);

    // Create GTK widgets
    GtkWidget *window = create_window();
    GtkWidget *scrolled_window = create_scrolled_window();
    GtkWidget *box = create_vertical_box();
    GtkWidget *textview = create_text_view();
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));

    // Create and configure the header label
    GtkWidget *header_label = gtk_label_new(NULL); 
    gtk_label_set_markup(GTK_LABEL(header_label), "<span foreground='purple'><big><b>Shortest Remaining Time First-SRT Algorithm</b></big></span>");
    gtk_box_pack_start(GTK_BOX(box), header_label, FALSE, FALSE, 7);

    // Insert a separator
    gtk_text_buffer_insert_at_cursor(buffer, "\n\n", -1);
    
/***********************________Order of execution________********************/  


    // Display order of execution
    display_order_of_execution_srt(box, processes, output, outputIndex);
    
/***********************________Process Information In Table________********************/   

    // Display Process Information Tree View
    GtkListStore *list_store;
    list_store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

    // Fill the list store with process information
    for (int i = 0; i < n; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(list_store, &iter);

        // Calculate turnaround time and waiting time
        int turnaroundTime = processes[i].completion_time - processes[i].arrive_time;
        int waitingTime = turnaroundTime - processes[i].burst;

        // Check if output[i] is a valid index
        if (output[i] >= 0 && output[i] < n) {
            gtk_list_store_set(list_store, &iter,
                               0, processes[i].id,
                               1, processes[i].arrive_time,
                               2, processes[i].burst,
                               3, waitingTime,
                               4, turnaroundTime,
                               5, processes[i].completion_time,
                               -1);
        } else {
            // Handle the case where output[i] is an invalid index
            g_print("Error: Invalid output index at position %d\n", i);
        }
    }

    // Create the tree view with columns for each piece of information
    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));

    // Add columns to the tree view with titles (header names)
    const char *column_names[] = {"Process", "Arrival Time", "Execution Time", "Waiting Time", "Turnaround Time", "Completion Time"};
    for (int i = 0; i < 6; i++) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes(column_names[i], renderer, "text", i, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    }

    // Attach the tree view to the box
    gtk_box_pack_start(GTK_BOX(box), tree_view, TRUE, TRUE, 0);
    
/***********************________Calculate and Display Averages ________********************/ 

    // Calculate and Display Averages
    char outputt[100]; // Adjust the size as needed
    int totalWaitingTime = 0, totalTurnaroundTime = 0; // Declare the variables here

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].completion_time - processes[i].arrive_time - processes[i].burst;
        totalTurnaroundTime += processes[i].completion_time - processes[i].arrive_time;
    }

    // Insert a separator
    gtk_text_buffer_insert_at_cursor(buffer, "\n\n", -1);

    // Display average waiting time label
    display_average_time_label(box, "Average Waiting Time (TAM)", totalWaitingTime, n);

    // Display average turnaround time label
    display_average_time_label(box, "Average Turnaround Time (TRM)", totalTurnaroundTime, n);

    // Pack the textview into the box
    gtk_box_pack_start(GTK_BOX(box), textview, TRUE, TRUE, 0);

    // Add the box to the scrolled window
    gtk_container_add(GTK_CONTAINER(scrolled_window), box);

    // Add the scrolled window to the window
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    // Show all widgets
    gtk_widget_show_all(window);

    // Main GTK loop
    gtk_main();
}

void display_order_of_execution_prprem(GtkWidget* box, Process tab2[], int output[], int outputIndex) {
    // Create a label for the order of execution
    GtkWidget* order_label = create_markup_label("<span foreground='green'><b>Order of Execution:</b></span>");
    gtk_box_pack_start(GTK_BOX(box), order_label, FALSE, FALSE, 0);

    // Create a grid for the order of execution
    GtkWidget* order_grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(order_grid), TRUE);
    gtk_box_pack_start(GTK_BOX(box), order_grid, TRUE, TRUE, 10);

    // Iterate through the output array to create the order of execution text
    for (int i = 0; i < outputIndex; i++) {
        // Create a label for the process
        GtkWidget* process_label = gtk_label_new(tab2[output[i] - 1].id);

        // Set background color based on process (customize the colors)
        GdkRGBA color;
        gdk_rgba_parse(&color, get_process_color_rr(output[i]));
        gtk_widget_override_background_color(process_label, GTK_STATE_FLAG_NORMAL, &color);

        // Pack the label into the grid
        gtk_grid_attach(GTK_GRID(order_grid), process_label, i, 0, 1, 1);
    }

    // Show all the widgets
    gtk_widget_show_all(box);
}



// Get process color for Round Robin based on process number
const char* get_process_color_prprem(int process_number) {
    switch (process_number) {
        case 1:
            return "#FFA07A";  // Light salmon for P1
        case 2:
            return "#32CD32";  // Lime green for P2
        case 3:
            return "#4682B4";  // Steel blue for P3
        case 4:
            return "#A9A9A9";  // Dark gray for P4
        default:
            return "#000000";  // Default to black for unknown processes
    }
}


/*================================================================*/
/**********________Priority_preemption_Scheduling_Interface________********/
/*================================================================*/

// Function to display the interface for Priority Scheduling algorithm
void display_prprem_interface(Process tab2[], int output[], int outputIndex, int process_count) {

    // Call the function to initialize GTK and create the main window
    initialize_and_create_window(&window, &scrolled_window, &box);

    // Create a label for the Priority Scheduling algorithm
    GtkWidget* pr_label = create_markup_label("<span foreground='purple'><big><b>Priority Scheduling Algorithm with preemption </b></big></span>");
    gtk_box_pack_start(GTK_BOX(box), pr_label, FALSE, FALSE, 0);
    
/***********************________Order of execution________********************/ 

  

    // Display order of execution
     display_order_of_execution_prprem( box, tab2,  output, outputIndex) ;
  
/***********************________Process Information In Table________********************/  
// Create a list store to hold the process information
GtkListStore *list_store;
list_store = gtk_list_store_new(7, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

// Fill the list store with process information
for (int i = 0; i < process_count; i++) {
    GtkTreeIter iter;
    gtk_list_store_append(list_store, &iter);

    // Calculate turnaround time and waiting time
   // int turnaroundTime = tab2[i].return_time - tab2[i].arrive_time;
   // int waitingTime = turnaroundTime - tab2[i].execution_time;

    gtk_list_store_set(list_store, &iter,
                       0, tab2[i].id,
                       1, tab2[i].arrive_time,
                       2, tab2[i].execution_time,
                       3, tab2[i].priority, // Add priority here
                       4, tab2[i].waiting_time,
                       5, tab2[i].turnaround_time,
                       6, tab2[i].return_time,
                       -1);
}

// Create the tree view with columns for each piece of information
GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
gtk_box_pack_start(GTK_BOX(box), tree_view, TRUE, TRUE, 0);

// Add columns to the tree view with titles (header names)
const char *column_names[] = {"Process", "Arrival Time", "Execution Time", "Priority", "Waiting Time", "Turnaround Time", "Completion Time"};
for (int i = 0; i < 7; i++) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes(column_names[i], renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
}

   
    /**************Correct***************/
/***********************________Calculate and Display Averages ________********************/ 

    // Calculate and Display Averages
    int totalWaitingTime = 0, totalTurnaroundTime = 0; // Declare the variables here

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < process_count; i++) {
        totalWaitingTime += tab2[i].waiting_time;
        totalTurnaroundTime += tab2[i].turnaround_time;
    }

    // Display average waiting time label
    display_average_time_label(box, "Average Waiting Time (TAM)", totalWaitingTime, process_count);

    // Display average turnaround time label
    display_average_time_label(box, "Average Turnaround Time (TRM)", totalTurnaroundTime, process_count);

    // Show all the widgets in the GTK window
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();
}




