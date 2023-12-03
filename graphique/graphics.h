#include <gtk/gtk.h>

#define ID_LEN 20

// Structure to represent a process
typedef struct _process {
    char id[ID_LEN];
    int arrive_time;
    int burst;
    int priority;
    int completed;  // RR  
    int return_time;  // RR
    int execution_time; // RR burst
    int reste;  // Priority Non-Preemptive
    int tempsfin; // Priority Non-Preemptive
    // SRT
    int remaining_time;
    int completion_time;
} Process;

// Function prototype for finding the index of the process with the shortest remaining time
int findShortestRemainingTimeIndex(Process processes[], int n, int currentTime);

// Declare global variables for GTK widgets
GtkWidget *window, *scrolled_window, *box, *tree_view, *textview, *text_view, *grid, *label;
GtkTextBuffer *buffer;

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
    gtk_widget_set_size_request(window, 800, 400);
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

// Function to display the order of execution for Priority Non-Preemptive
void display_order_of_execution_pr(GtkWidget* box, Process tab2[], int output[], int outputIndex) {
    // Create a label for the order of execution
    GtkWidget* order_label = create_markup_label("<span foreground='purple'><big><b>Order of Execution:</b></big></span>");
    gtk_box_pack_start(GTK_BOX(box), order_label, FALSE, FALSE, 0);

    // Create a text view for displaying the order of execution
    GtkWidget* order_text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(order_text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(order_text_view), FALSE);

    // Get the buffer associated with the text view
    GtkTextBuffer* order_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(order_text_view));

    // Initialize the order_text with a starting symbol
    gchar* order_text = g_strdup("| ");

    // Iterate through the output array to create the order of execution text
    for (int i = 0; i < outputIndex; i++) {
        gchar* process_text = g_strdup_printf("%s", tab2[output[i]].id);
        order_text = g_strconcat(order_text, process_text, " -------- ", NULL);
        g_free(process_text);
    }

    // Insert the order_text into the text buffer
    gtk_text_buffer_insert_at_cursor(order_buffer, order_text, -1);
    g_free(order_text);

    // Pack the order text view into the GTK box
    gtk_box_pack_start(GTK_BOX(box), order_text_view, FALSE, FALSE, 10);
}

// Function to display the average time label
void display_average_time_label(GtkWidget* box, const char* label_text, int total_time, int n) {
    // Create a label for Average Time
    GtkWidget* label = gtk_label_new(NULL);
    gchar* labelMarkup = g_strdup_printf("<span foreground='green'><big><b>%s:</b></big></span>", label_text);
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

// Function to display the order of execution for Round Robin
void display_order_of_execution_rr(GtkWidget* box, Process tab2[], int output[], int outputIndex) {
    // Create a label for the order of execution
    GtkWidget *order_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(order_label), "<span foreground='purple'><big><b>Order of Execution:</b></big></span>");
    gtk_box_pack_start(GTK_BOX(box), order_label, FALSE, FALSE, 7);
    
    // Create a text view for the order of execution
    GtkWidget *order_text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(order_text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(order_text_view), FALSE);

    // Create a text buffer for the order of execution
    GtkTextBuffer *order_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(order_text_view));

    // Display the order of execution
    for (int i = 0; i < outputIndex; i++) {
        gchar *process_text = g_strdup_printf("| P%d -------- ", output[i]);
        gtk_text_buffer_insert_at_cursor(order_buffer, process_text, -1);
        g_free(process_text);
    }

    // Pack the text view into the box
    gtk_box_pack_start(GTK_BOX(box), order_text_view, FALSE, FALSE, 10);
}

// Function to find the index of the process with the shortest remaining time
int findShortestRemainingTimeIndex(Process processes[], int n, int currentTime) {
    int shortestIndex = -1;
    int shortestTime = INT_MAX;

    // Iterate through the processes
    for (int i = 0; i < n; i++) {
        // Check if the process has arrived, has remaining time, and has shorter remaining time
        if (processes[i].arrive_time <= currentTime &&
            processes[i].remaining_time < shortestTime &&
            processes[i].remaining_time > 0) {
            shortestIndex = i;
            shortestTime = processes[i].remaining_time;
        }
    }

    return shortestIndex;
}

