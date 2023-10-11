#include <iostream>
#include <gtk/gtk.h>
#include "clientNetwork.hpp"
#include <thread>
using namespace std;

// User adds contents to this entry.
static GtkWidget *input = NULL;

// The text buffer containing all previous user entries.
static GtkTextBuffer *buffer = NULL;

// When the "userText" button is clicked, all the user added to the
// entry is transfered to the text area.
static Client client;

void button_clicked(GtkWidget *widget, gpointer data)
{
    // Get the text added to the entry by the user:
    const gchar *userText;
    userText = gtk_entry_get_text(GTK_ENTRY(input));

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);

    sf::Packet packet;
    packet << userText;
    client.sendPacket(packet);

    gtk_text_buffer_insert(buffer, &end, userText, -1);
    gtk_text_buffer_insert(buffer, &end, "\n", -1);

    // Clear the user input entry for the next input:
    gtk_entry_set_text(GTK_ENTRY(input), "");
}


void getMessage()
{
    while(true)
    {
        if(client.recivePacket() == sf::Socket::Done)
        {
            gchar* message;
            sf::Packet packet = std::move(client.getPacket());
            packet >> message;

            GtkTextIter end;
            gtk_text_buffer_get_end_iter(buffer, &end);

            gtk_text_buffer_insert(buffer, &end, message, -1);
            gtk_text_buffer_insert(buffer, &end, "\n", -1);

        }
    }
}

int main(int argc, char** argv)
{
    client.connected("localhost", 1200);
     gtk_init(&argc,&argv);

    GtkWidget *mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    input = gtk_entry_new();

    GtkWidget *send = gtk_button_new_with_label("Send");

    // A text view is used to show the user's past input (in terminal
    // fashion). We wrap the text view in a GtkScrollmainWindowdow, which
    // makes the text view's content scrollable:
    GtkWidget *scrollWindow = gtk_scrolled_window_new(NULL, NULL);
    buffer = gtk_text_buffer_new(NULL);
    GtkWidget *textArea = gtk_text_view_new_with_buffer(buffer);
    gtk_container_add(GTK_CONTAINER(scrollWindow), textArea);

    g_signal_connect(send,"clicked", G_CALLBACK(button_clicked), NULL);

    GtkWidget *layout = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(layout), scrollWindow, 1, 1, 25, 1);
    gtk_grid_attach(GTK_GRID(layout), input, 1, 10, 1, 1);
    gtk_grid_attach(GTK_GRID(layout), send, 2, 10, 1, 1);
    gtk_container_set_focus_child(GTK_CONTAINER(layout), input);

    gtk_container_add(GTK_CONTAINER(mainWindow),layout);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 20);
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(mainWindow);

    std::thread receptionThread(getMessage);

    gtk_main();

    return  0;

}
