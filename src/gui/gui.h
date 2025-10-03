#ifndef GUI_H
#define GUI_H

#ifdef HAVE_GTK
#include <gtk/gtk.h>
#include "../common/types.h"
#include "../engine/attack_engine.h"

typedef struct {
    GtkWidget *window;
    GtkWidget *hash_entry;
    GtkWidget *target_combo;
    GtkWidget *method_combo;
    GtkWidget *max_length_spin;
    GtkWidget *max_attempts_spin;
    GtkWidget *wordlist_entry;
    GtkWidget *special_chars_check;
    GtkWidget *start_button;
    GtkWidget *stop_button;
    GtkWidget *progress_bar;
    GtkWidget *status_label;
    GtkWidget *result_label;
    GtkWidget *attempts_label;
    GtkWidget *rate_label;
    
    attack_config_t current_config;
    char current_hash[256];
    bool attack_running;
    GThread *attack_thread;
} CrasherGUI;

int gui_main(void);
void* attack_thread_func(void* data);

void on_start_clicked(GtkWidget *widget, gpointer data);
void on_stop_clicked(GtkWidget *widget, gpointer data);
void on_method_changed(GtkWidget *widget, gpointer data);
gboolean update_progress(gpointer data);

#else

int gui_main(void);

#endif

#endif
