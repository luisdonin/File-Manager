#include <gtk/gtk.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void create_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Create a file", GTK_WINDOW(data), action, "_Cancel", GTK_RESPONSE_CANCEL, "_Create", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        ofstream file(filename);
        file.close();
        g_free(filename);
        gtk_widget_destroy(dialog);
        GtkWidget *message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "File created successfully.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
    } else {
        gtk_widget_destroy(dialog);
    }
}

void read_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Read a file", GTK_WINDOW(data), action, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        ifstream file(filename);
        if (file.is_open()) {
            string content;
            while (getline(file, content)) {
                cout << content << endl;
            }
            file.close();
            g_free(filename);
            gtk_widget_destroy(dialog);
            GtkWidget *message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, content.c_str());
            gtk_dialog_run(GTK_DIALOG(message));
            gtk_widget_destroy(message);
        } else {
            file.close();
            g_free(filename);
            gtk_widget_destroy(dialog);
            GtkWidget *message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Unable to open file.");
            gtk_dialog_run(GTK_DIALOG(message));
            gtk_widget_destroy(message);
        }
    } else {
        gtk_widget_destroy(dialog);
    }
}

void write_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Write to a file", GTK_WINDOW(data), action, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            GtkWidget *content_dialog = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Enter the content:");
            gtk_window_set_title(GTK_WINDOW(content_dialog), "Write to a file");
            GtkResponseType content_res = gtk_dialog_run(GTK_DIALOG(content_dialog));
            if (content_res == GTK_RESPONSE_OK) {
                const char *content = gtk_message_dialog_get_text(GTK_MESSAGE_DIALOG(content_dialog));
                file << content << endl;
                GtkWidget *message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Content written to file successfully.");
                gtk_dialog_run(GTK_DIALOG(message));
                gtk_widget_destroy(message);
            }
            gtk_widget_destroy(content_dialog);
            file.close();
            g_free(filename);
            gtk_widget_destroy(dialog);
        } else {
            file.close();
            g_free(filename);
            gtk_widget_destroy(dialog);
            GtkWidget *message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Unable to open file.");
            gtk_dialog_run(GTK_DIALOG(message));
            gtk_widget_destroy(message);
        }
    } else {
        gtk_widget_destroy(dialog);
    }
}

void delete_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Delete a file", GTK_WINDOW(data), action, "_Cancel", GTK_RESPONSE_CANCEL, "_Delete", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        if (remove(filename) == 0) {
            g_free(filename);
            gtk_widget_destroy(dialog);
            GtkWidget *message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "File deleted successfully.");
            gtk_dialog_run(GTK_DIALOG(message));
            gtk_widget_destroy(message);
        } else {
            g_free(filename);
            gtk_widget_destroy(dialog);
            GtkWidget *message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "Unable to delete file.");
            gtk_dialog_run(GTK_DIALOG(message));
            gtk_widget_destroy(message);
        }
    } else {
        gtk_widget_destroy(dialog);
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *create_button;
    GtkWidget *read_button;
    GtkWidget *write_button;
    GtkWidget *delete_button;
    GtkWidget *exit_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Manager");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    create_button = gtk_button_new_with_label("Create a file");
    g_signal_connect(create_button, "clicked", G_CALLBACK(create_file), window);
    gtk_grid_attach(GTK_GRID(grid), create_button, 0, 0, 1, 1);

    read_button = gtk_button_new_with_label("Read a file");
    g_signal_connect(read_button, "clicked", G_CALLBACK(read_file), window);
    gtk_grid_attach(GTK_GRID(grid), read_button, 1, 0, 1, 1);

    write_button = gtk_button_new_with_label("Write to a file");
    g_signal_connect(write_button, "clicked", G_CALLBACK(write_file), window);
    gtk_grid_attach(GTK_GRID(grid), write_button, 0, 1, 1, 1);

    delete_button = gtk_button_new_with_label("Delete a file");
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_file), window);
    gtk_grid_attach(GTK_GRID(grid), delete_button, 1, 1, 1, 1);

    exit_button = gtk_button_new_with_label("Exit");
    g_signal_connect(exit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_grid_attach(GTK_GRID(grid), exit_button, 0, 2, 2, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}