#ifndef SJF_H
#define SJF_H

#include <gtk/gtk.h>

#define ID_LEN 20

typedef struct _process {
    char id[ID_LEN];
    int arrive_time;
    int burst;
    int waitingTime;
    int tempsfin;
    int turnaroundTime;
    GdkRGBA color;
    int start_time;
    int finish_time;
} Process;

#define MAX_PROCESSES 100

Process process[MAX_PROCESSES];

void displayGanttChart(Process *process, int np) {
    printf("\n--- Gantt Chart ---\n");
    for (int i = 0; i < np; i++) {
        printf("| %s ", process[i].id);
    }
    printf("|\n");
}

void displayProcessTable(Process *process, int np) {
    printf("\n--- Process Table ---\n");
    printf("+-----+-------------+------------+--------------+-----------------+-----------------+\n");
    printf("| PID | Arrive time | Burst Time | Waiting Time | Turnaround Time | Completion Time |\n");
    printf("+-----+-------------+------------+--------------+-----------------+-----------------+\n");

    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    for (int i = 0; i < np; i++) {
        totalWaitingTime += process[i].waitingTime;
        totalTurnaroundTime += process[i].turnaroundTime;

        printf("| %-3s | %-11d | %-10d | %-12d | %-15d | %-15d |\n",
               process[i].id, process[i].arrive_time, process[i].burst,
               process[i].waitingTime, process[i].turnaroundTime, process[i].tempsfin);
    }

    printf("+-----+-------------+------------+--------------+-----------------+-----------------+\n");

    printf("\nTotal Waiting Time      : %d\n", totalWaitingTime);
    printf("Average Waiting Time    : %.2f\n", (float)totalWaitingTime / np);
    printf("Total Turnaround Time   : %d\n", totalTurnaroundTime);
    printf("Average Turnaround Time : %.2f\n", (float)totalTurnaroundTime / np);
}

int compareBurstTime(const void *a, const void *b) {
    const Process *process_a = (const Process *)a;
    const Process *process_b = (const Process *)b;

    int arrive_time_a = (*process_a).arrive_time;
    int arrive_time_b = (*process_b).arrive_time;
    int burst_a = (*process_a).burst;
    int burst_b = (*process_b).burst;

    if (arrive_time_a != arrive_time_b) {
        return arrive_time_a - arrive_time_b;
    }

    return burst_a - burst_b;
}

static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    Process *process_data = (Process *)data;

    double x = 50.0, y = 50.0;
    double width, height = 30.0;

    for (int i = 0; process_data[i].id[0] != '\0'; i++) {
        width = process_data[i].burst * 50.0;

        gdk_cairo_set_source_rgba(cr, &process_data[i].color);

        cairo_rectangle(cr, x, y, width, height);
        cairo_fill_preserve(cr);
        cairo_stroke(cr);

        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 17.0);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

        cairo_move_to(cr, x + width / 2.0 - 10, y + height / 2.0);
        cairo_show_text(cr, process_data[i].id);

        x += width;
    }

    return FALSE;
}

GtkWidget* create_markup_label(const char* markup) {
    GtkWidget* label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), markup);
    return label;
}

static void show_table_SJF(GtkWidget *widget, gpointer data) {
    Process *process_data = (Process *)data;

    GtkWidget *table_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(table_window), "Table_SJF");
    gtk_window_set_default_size(GTK_WINDOW(table_window), 500, 250);

    GtkWidget *vboxx = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget* pr_label = create_markup_label("<span foreground='purple'><big><b>Shortest Job First Algorithm (SJF) </b></big></span>");
    gtk_box_pack_start(GTK_BOX(vboxx), pr_label, FALSE, FALSE, 0);

    GtkListStore *list_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
    GtkTreeIter iter;

    int prev_completion_time = 0;
    int total_waiting_time = 0, total_turnaround_time = 0;
    int num_processes = 0;

    for (int i = 0; process_data[i].id[0] != '\0'; i++) {
        gtk_list_store_append(list_store, &iter);
        int completion_time = MAX(process_data[i].arrive_time, prev_completion_time) + process_data[i].burst;
        gtk_list_store_set(list_store, &iter, 0, process_data[i].id, 1, process_data[i].arrive_time, 2, process_data[i].burst, 3, completion_time, -1);

        prev_completion_time = completion_time;

        int waiting_time = MAX(0, (prev_completion_time - process_data[i].arrive_time - process_data[i].burst));
        total_waiting_time += waiting_time;
        total_turnaround_time += (waiting_time + process_data[i].burst);
        num_processes++;
    }

    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);

    char *column_names[] = {"ID", "Arrival Time", "Burst Time", "Completion Time"};
    for (int i = 0; i < 4; i++) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        g_object_set(renderer, "xalign", 0.3, "xpad", 8, NULL);
        GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(column_names[i], renderer, "text", i, NULL);
        gtk_tree_view_column_set_spacing(column, 40);
        gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    }

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), tree_view);

    char average_waiting_time[100], average_turnaround_time[100];
    if (num_processes > 0) {
        snprintf(average_waiting_time, sizeof(average_waiting_time),
                 "<span foreground='green'><b>Average Waiting Time (TAM):</b></span>%.2f", (float)total_waiting_time / num_processes);
        snprintf(average_turnaround_time, sizeof(average_turnaround_time),
                 "<span foreground='green'><b>Average Turnaround Time (TRM):</b></span>%.2f", (float)total_turnaround_time / num_processes);
    } else {
        snprintf(average_waiting_time, sizeof(average_waiting_time),
                 "<span foreground='green'><b>Average Waiting Time (TAM): N/A</b></span>");
        snprintf(average_turnaround_time, sizeof(average_turnaround_time),
                 "<span foreground='green'><b>Average Turnaround Time (TRM): N/A</b></span>");
    }

    GtkWidget *table_label_tam = gtk_label_new(NULL);
    GtkWidget *table_label_trm = gtk_label_new(NULL);

    gtk_label_set_markup(GTK_LABEL(table_label_tam), average_waiting_time);
    gtk_label_set_markup(GTK_LABEL(table_label_trm), average_turnaround_time);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), table_label_tam, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), table_label_trm, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(table_window), vboxx);
    gtk_box_pack_start(GTK_BOX(vboxx), vbox, TRUE, TRUE, 0);

    gtk_widget_show_all(table_window);
}

static void show_diagram_SJF(GtkWidget *widget, gpointer data) {
    Process *process_data = (Process *)data;

    GtkWidget *diagram_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(diagram_window), "Gantt Chart");
    gtk_window_set_default_size(GTK_WINDOW(diagram_window), 1000, 200);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *title_label = gtk_label_new(NULL);
    const gchar *title_markup = "<span foreground='purple' font_desc='Bold 15'>Order of execution _SJF_</span>";
    gtk_label_set_markup(GTK_LABEL(title_label), title_markup);
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 0);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(diagram_window), vbox);

    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), process_data);

    gtk_widget_show_all(diagram_window);
}


void initializeGUI_SJF(int argc, char *argv[], Process *process, int np) {
    qsort(process, np, sizeof(Process), compareBurstTime);

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), " Processes_Information_SJF ");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *table_button = gtk_button_new_with_label("Table");
    GtkWidget *diagram_button = gtk_button_new_with_label("Gantt Chart");

    g_signal_connect(G_OBJECT(table_button), "clicked", G_CALLBACK(show_table_SJF), process);
    g_signal_connect(G_OBJECT(diagram_button), "clicked", G_CALLBACK(show_diagram_SJF), process);

    gtk_box_pack_start(GTK_BOX(main_box), table_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), diagram_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    gtk_main();
}

void parseProcessColor(Process *process, int np) {
    switch (np % 4) {
        case 0:
            gdk_rgba_parse(&process[np].color, "#ff9999"); // Light Red
            break;
        case 1:
            gdk_rgba_parse(&process[np].color, "#99ccff"); // Light Blue
            break;
        case 2:
            gdk_rgba_parse(&process[np].color, "#99ffcc"); // Light Green
            break;
        case 3:
            gdk_rgba_parse(&process[np].color, "#ffcc99"); // Light Orange
            break;
        default:
            // Default color in case the index is out of range
            gdk_rgba_parse(&process[np].color, "#000000"); // Black
            break;
    }
}



#endif // SJF_H

