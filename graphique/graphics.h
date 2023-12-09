#include <gtk/gtk.h>
#ifndef GRAPHICS_H
#define GRAPHICS_H

#define ID_LEN 20
#define MAX 100
#define ID_LEN 20
#define TRUE 1
#define FALSE 0

// Structure to represent a process
typedef struct _process {
    char id[ID_LEN];
    int arrive_time;
    int burst;
    int priority;
    int completed;  // For RR  
    int return_time;  // For RR
    int execution_time; // For RR burst
    int reste;  // For Priority Non-Preemptive
    int tempsfin; // For Priority Non-Preemptive
    // For SRT
    int remaining_time;
    int completion_time;
    //for prprem
    int waiting_time;
    int turnaround_time;
    int response_time;
} Process;

// Forward declaration for process color functions
const char* get_process_color(const char* process_id);
const char* get_process_color_rr(int process_number);
const char* get_process_color_srt(int process_number);

// Declare global variables for GTK widgets
GtkWidget *window, *scrolled_window, *box, *tree_view, *textview, *text_view, *grid, *label;
GtkTextBuffer *buffer;
gchar *text;

// Function to create a label with markup
GtkWidget* create_markup_label(const char* markup) {
    GtkWidget* label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), markup);
    return label;
}

// Helper function to create a GTK window
GtkWidget* create_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Processes Information");
    gtk_widget_set_size_request(window, 800, 450);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    return window;
}

// Helper function to create a GTK scrolled window
GtkWidget* create_scrolled_window() {
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    return scrolled_window;
}

// Helper function to create a GTK vertical box
GtkWidget* create_vertical_box() {
    return gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
}

// Helper function to create a GTK text view
GtkWidget* create_text_view() {
    GtkWidget *textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
    return textview;
}

// Function to initialize GTK and create the main window
void initialize_and_create_window(GtkWidget **window, GtkWidget **scrolled_window, GtkWidget **box) {
    // Initialize GTK
    if (!gtk_init_check(NULL, NULL)) {
        g_error("Failed to initialize GTK.");
        return;
    }

    // Create the main window
    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(*window), "Process Information");
    gtk_widget_set_size_request(*window, 800, 500);
    g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a scrolled window for displaying content
    *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(*scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(*window), *scrolled_window);

    // Create a vertical box to organize widgets
    *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(*scrolled_window), *box);
}

// Function to display the average time label
void display_average_time_label(GtkWidget* box, const char* label_text, int total_time, int n) {
    // Create a label for Average Time
    GtkWidget* label = gtk_label_new(NULL);
    gchar* labelMarkup = g_strdup_printf("<span foreground='green'><b>%s:</b></span>", label_text);
    gtk_label_set_markup(GTK_LABEL(label), labelMarkup);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // Display average time value
    gchar output[100];
    snprintf(output, sizeof(output), "%.2f", (float)total_time / n);
    gchar* valueMarkup = g_strdup_printf("<span>%s</span>", output);
    GtkWidget* valueLabel = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(valueLabel), valueMarkup);
    gtk_box_pack_start(GTK_BOX(box), valueLabel, FALSE, FALSE, 5); // Adjust the spacing as needed

    g_free(labelMarkup);
    g_free(valueMarkup);
}

/****************************************Order of execution**********************************/

// Function to display the order of execution for Priority Non-Preemptive
void display_order_of_execution_pr(GtkWidget* box, Process tab2[], int output[], int outputIndex) {
    // Create a label for the order of execution
    GtkWidget* order_label = create_markup_label("<span foreground='green'><b>Order of Execution:</b></span>");
    gtk_box_pack_start(GTK_BOX(box), order_label, FALSE, FALSE, 0);
    
    // Create a grid for the order of executions
    GtkWidget* order_grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(order_grid), TRUE);
    gtk_box_pack_start(GTK_BOX(box), order_grid, FALSE, FALSE, 10);

    // Iterate through the output array to create the order of execution text
    for (int i = 0; i < outputIndex; i++) {
        // Create a label for the process
        GtkWidget* process_label = gtk_label_new(tab2[output[i]].id);
        
        // Set background color based on process (customize the colors)
        GdkRGBA color;
        gdk_rgba_parse(&color, get_process_color(tab2[output[i]].id));
        gtk_widget_override_background_color(process_label, GTK_STATE_FLAG_NORMAL, &color);

        // Pack the label into the grid
        gtk_grid_attach(GTK_GRID(order_grid), process_label, i, 0, 1, 1);
    }

    // Show all the widgets
    gtk_widget_show_all(box);
}

// Function to get process color based on process ID
const char* get_process_color(const char* process_id) {
    // Assign colors based on process_id
    if (strcmp(process_id, "P1") == 0) {
        return "#FF6347";  // Tomato red for P1
    } else if (strcmp(process_id, "P2") == 0) {
        return "#FFD700";  // Gold for P2
    } else if (strcmp(process_id, "P3") == 0) {
        return "#00CED1";  // Dark turquoise for P3
    } else if (strcmp(process_id, "P4") == 0) {
        return "#9370DB";  // Medium purple for P4
    } else {
        return "#CCCCCC";  // Default color for unknown processes
    }
}

// Display the order of execution for Round Robin
void display_order_of_execution_rr(GtkWidget* box, Process tab2[], int output[], int outputIndex) {
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
const char* get_process_color_rr(int process_number) {
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

// Display the order of execution for Shortest Remaining Time
void display_order_of_execution_srt(GtkWidget* box, Process processes[], int output[], int outputIndex) {
    // Create a label for the order of execution
    GtkWidget* order_label = create_markup_label("<span foreground='green'><b>Order of Execution:</b></span>");
    gtk_box_pack_start(GTK_BOX(box), order_label, FALSE, FALSE, 0);

    // Create a grid to display the processes
    GtkWidget *order_grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(order_grid), TRUE);
    gtk_box_pack_start(GTK_BOX(box), order_grid, TRUE, TRUE, 10);

    // Iterate through the output sequence
    for (int i = 0; i < outputIndex; i++) {
        GtkWidget *process_label = gtk_label_new(processes[output[i]].id);
        
        // Set background color based on process
        GdkRGBA color;
        gdk_rgba_parse(&color, get_process_color_srt(output[i] + 1));  // Adjusting the process index by 1
        gtk_widget_override_background_color(process_label, GTK_STATE_FLAG_NORMAL, &color);

        // Add label to the grid
        gtk_grid_attach(GTK_GRID(order_grid), process_label, i, 0, 1, 1); // Attach to the grid at position (i, 0)
    }

    // Show all widgets
    gtk_widget_show_all(box);
}

// Get process color for Shortest Remaining Time based on process number
const char* get_process_color_srt(int process_number) {
    switch (process_number) {
        case 1:
            return "#FF6F61";  // Coral red for P1
        case 2:
            return "#70A288";  // Sage green for P2
        case 3:
            return "#6B5B95";  // Royal purple for P3
        case 4:
            return "#FFB14E";  // Peach for P4
        default:
            return "#000000";  // Default to black for unknown processes
    }
}


#endif // GRAPHICS_H

