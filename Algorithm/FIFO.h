#include "SJF.h"

int compareArrivalTime(const void *a, const void *b) {
    return ((Process *)a)->arrive_time - ((Process *)b)->arrive_time;
}

GdkRGBA timeline_color = {0.0, 0.0, 0.0, 0.5};

void FIFO(Process *process, int np) {
    qsort(process, np, sizeof(Process), compareArrivalTime);

    int currentTime = 0;
    for (int i = 0; i < np; i++) {
        process[i].tempsfin = currentTime + process[i].burst;
        process[i].waitingTime = currentTime - process[i].arrive_time;
        process[i].turnaroundTime = process[i].waitingTime + process[i].burst;
        currentTime = process[i].tempsfin;
    }
}

static void show_table_FIFO(GtkWidget *widget, gpointer data) {
    Process *process_data = (Process *)data;

    GtkWidget *table_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(table_window), "Table_FIFO");
    gtk_window_set_default_size(GTK_WINDOW(table_window), 500, 250);

    GtkWidget *vboxx = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *pr_label = create_markup_label("<span foreground='purple'><big><b>First In First Out Algorithm (FIFO) </b></big></span>");
    gtk_box_pack_start(GTK_BOX(vboxx), pr_label, FALSE, FALSE, 0);

    GtkListStore *list_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
    GtkTreeIter iter;

    int total_waiting_time = 0, total_turnaround_time = 0;
    int num_processes = 0;

    for (int i = 0; process_data[i].id[0] != '\0'; i++) {
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, process_data[i].id,
                                        1, process_data[i].arrive_time,
                                        2, process_data[i].burst,
                                        3, process_data[i].finish_time, -1);

        total_waiting_time += process_data[i].waitingTime;
        total_turnaround_time += process_data[i].turnaroundTime;
        num_processes++;
    }

    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);

    const char *columns[] = {"Process ID", "Arrival Time", "Execution Time", "Completion Time"};

    for (int i = 0; i < 4; i++) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(columns[i], renderer, "text", i, NULL);
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

static void show_diagram_FIFO(GtkWidget *widget, gpointer data) {
    Process *process_data = (Process *)data;

    int total_time = 0;
    for (int i = 0; process_data[i].id[0] != '\0'; i++) {
        total_time += process_data[i].burst;
    }

    GtkWidget *diagram_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(diagram_window), "Gantt Diagram");
    gtk_window_set_default_size(GTK_WINDOW(diagram_window), 1000, 200);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *title_label = gtk_label_new(NULL);
    const gchar *title_markup = "<span foreground='purple' font_desc='Bold 15'>Order of execution _FIFO_</span>";
    gtk_label_set_markup(GTK_LABEL(title_label), title_markup);
    gtk_box_pack_start(GTK_BOX(vbox), title_label, FALSE, FALSE, 0);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(diagram_window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), process_data);

    gtk_widget_show_all(diagram_window);
}

void initializeGUI_FIFO(int argc, char *argv[], Process *process, int np) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), " Processes_Information_FIFO ");
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *table_button = gtk_button_new_with_label("Table");
    GtkWidget *diagram_button = gtk_button_new_with_label("Gantt Chart");

    g_signal_connect(G_OBJECT(table_button), "clicked", G_CALLBACK(show_table_FIFO), process);
    g_signal_connect(G_OBJECT(diagram_button), "clicked", G_CALLBACK(show_diagram_FIFO), process);

    gtk_box_pack_start(GTK_BOX(main_box), table_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), diagram_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    gtk_main();
}

