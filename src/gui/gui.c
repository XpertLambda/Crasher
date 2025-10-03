#include "gui.h"
#include <stdio.h>

#ifdef HAVE_GTK

#include "../engine/attack_engine.h"
#include "../common/utils.h"
#include <pthread.h>

static CrasherGUI *g_gui = NULL;
static pthread_t attack_thread;
static volatile gboolean g_stop_attack = FALSE;

static void on_start_clicked(GtkWidget *widget, gpointer data);
static void on_stop_clicked(GtkWidget *widget, gpointer data);
static void on_method_changed(GtkWidget *widget, gpointer data);
static gboolean update_progress(gpointer data);
static void *attack_worker(void *data);

void gui_init(CrasherGUI *gui) {
    gtk_init(NULL, NULL);
    
    gui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(gui->window), "Crasher - Professional Hash Cracking Tool");
    gtk_window_set_default_size(GTK_WINDOW(gui->window), 600, 500);
    gtk_window_set_resizable(GTK_WINDOW(gui->window), FALSE);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 20);
    gtk_container_add(GTK_CONTAINER(gui->window), main_box);
    
    // Header
    GtkWidget *header = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(header), 
        "<span size='large' weight='bold'>🔐 Crasher Professional</span>\n"
        "<span size='small'>Advanced Hash Cracking Toolkit</span>");
    gtk_box_pack_start(GTK_BOX(main_box), header, FALSE, FALSE, 0);
    
    // Hash input section
    GtkWidget *hash_frame = gtk_frame_new("Target Hash");
    gtk_box_pack_start(GTK_BOX(main_box), hash_frame, FALSE, FALSE, 0);
    
    GtkWidget *hash_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(hash_box), 10);
    gtk_container_add(GTK_CONTAINER(hash_frame), hash_box);
    
    gui->hash_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(gui->hash_entry), 
        "Enter MD5 (32), SHA-1 (40), or SHA-256 (64) hash...");
    gtk_box_pack_start(GTK_BOX(hash_box), gui->hash_entry, FALSE, FALSE, 0);
    
    // Configuration section
    GtkWidget *config_frame = gtk_frame_new("Attack Configuration");
    gtk_box_pack_start(GTK_BOX(main_box), config_frame, FALSE, FALSE, 0);
    
    GtkWidget *config_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(config_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(config_grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(config_grid), 10);
    gtk_container_add(GTK_CONTAINER(config_frame), config_grid);
    
    // Target type
    GtkWidget *target_label = gtk_label_new("Target Type:");
    gtk_grid_attach(GTK_GRID(config_grid), target_label, 0, 0, 1, 1);
    
    gui->target_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->target_combo), "Password");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->target_combo), "PIN");
    gtk_combo_box_set_active(GTK_COMBO_BOX(gui->target_combo), 0);
    gtk_grid_attach(GTK_GRID(config_grid), gui->target_combo, 1, 0, 1, 1);
    
    // Attack method
    GtkWidget *method_label = gtk_label_new("Attack Method:");
    gtk_grid_attach(GTK_GRID(config_grid), method_label, 0, 1, 1, 1);
    
    gui->method_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->method_combo), "Dictionary");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->method_combo), "Brute Force");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->method_combo), "Random");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->method_combo), "Auto");
    gtk_combo_box_set_active(GTK_COMBO_BOX(gui->method_combo), 3);
    gtk_grid_attach(GTK_GRID(config_grid), gui->method_combo, 1, 1, 1, 1);
    
    g_signal_connect(gui->method_combo, "changed", G_CALLBACK(on_method_changed), gui);
    
    // Wordlist directory
    GtkWidget *wordlist_label = gtk_label_new("Wordlist Directory:");
    gtk_grid_attach(GTK_GRID(config_grid), wordlist_label, 0, 2, 1, 1);
    
    gui->wordlist_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(gui->wordlist_entry), ".");
    gtk_grid_attach(GTK_GRID(config_grid), gui->wordlist_entry, 1, 2, 1, 1);
    
    // Max length
    GtkWidget *length_label = gtk_label_new("Max Length:");
    gtk_grid_attach(GTK_GRID(config_grid), length_label, 0, 3, 1, 1);
    
    gui->max_length_spin = gtk_spin_button_new_with_range(1, 12, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui->max_length_spin), 8);
    gtk_grid_attach(GTK_GRID(config_grid), gui->max_length_spin, 1, 3, 1, 1);
    
    // Max attempts
    GtkWidget *attempts_label = gtk_label_new("Max Attempts:");
    gtk_grid_attach(GTK_GRID(config_grid), attempts_label, 0, 4, 1, 1);
    
    gui->max_attempts_spin = gtk_spin_button_new_with_range(1000, 10000000, 1000);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui->max_attempts_spin), 1000000);
    gtk_grid_attach(GTK_GRID(config_grid), gui->max_attempts_spin, 1, 4, 1, 1);
    
    // Special characters
    gui->special_chars_check = gtk_check_button_new_with_label("Include Special Characters");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->special_chars_check), TRUE);
    gtk_grid_attach(GTK_GRID(config_grid), gui->special_chars_check, 0, 5, 2, 1);
    
    // Progress section
    GtkWidget *progress_frame = gtk_frame_new("Progress");
    gtk_box_pack_start(GTK_BOX(main_box), progress_frame, FALSE, FALSE, 0);
    
    GtkWidget *progress_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(progress_box), 10);
    gtk_container_add(GTK_CONTAINER(progress_frame), progress_box);
    
    gui->progress_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(gui->progress_bar), TRUE);
    gtk_box_pack_start(GTK_BOX(progress_box), gui->progress_bar, FALSE, FALSE, 0);
    
    // Results section
    GtkWidget *result_frame = gtk_frame_new("Results");
    gtk_box_pack_start(GTK_BOX(main_box), result_frame, TRUE, TRUE, 0);
    
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), 
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(result_frame), scroll);
    
    gui->result_label = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(gui->result_label), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(gui->result_label), FALSE);
    gtk_container_add(GTK_CONTAINER(scroll), gui->result_label);
    
    // Control buttons
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);
    
    gui->start_button = gtk_button_new_with_label("🚀 Start Attack");
    gtk_widget_set_size_request(gui->start_button, 150, 40);
    g_signal_connect(gui->start_button, "clicked", G_CALLBACK(on_start_clicked), gui);
    gtk_box_pack_start(GTK_BOX(button_box), gui->start_button, FALSE, FALSE, 0);
    
    gui->stop_button = gtk_button_new_with_label("⏹️ Stop");
    gtk_widget_set_size_request(gui->stop_button, 100, 40);
    gtk_widget_set_sensitive(gui->stop_button, FALSE);
    g_signal_connect(gui->stop_button, "clicked", G_CALLBACK(on_stop_clicked), gui);
    gtk_box_pack_start(GTK_BOX(button_box), gui->stop_button, FALSE, FALSE, 0);
    
    GtkWidget *quit_button = gtk_button_new_with_label("❌ Exit");
    gtk_widget_set_size_request(quit_button, 100, 40);
    g_signal_connect(quit_button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_end(GTK_BOX(button_box), quit_button, FALSE, FALSE, 0);
    
    g_signal_connect(gui->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    on_method_changed(gui->method_combo, gui);
    
    gtk_widget_show_all(gui->window);
}

static void append_result_text(CrasherGUI *gui, const char *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gui->result_label));
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buffer, &end);
    gtk_text_buffer_insert(buffer, &end, text, -1);
    gtk_text_buffer_insert(buffer, &end, "\n", -1);
    
    GtkTextMark *mark = gtk_text_buffer_get_insert(buffer);
    gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(gui->result_label), mark);
}

static void on_method_changed(GtkWidget *widget, gpointer data) {
    CrasherGUI *gui = (CrasherGUI*)data;
    int method = gtk_combo_box_get_active(GTK_COMBO_BOX(widget));
    
    gtk_widget_set_sensitive(gui->wordlist_entry, method == 0 || method == 3);
    gtk_widget_set_sensitive(gui->max_length_spin, method == 1 || method == 2 || method == 3);
    gtk_widget_set_sensitive(gui->max_attempts_spin, method == 2 || method == 3);
    gtk_widget_set_sensitive(gui->special_chars_check, method == 1 || method == 2 || method == 3);
}

static void on_start_clicked(GtkWidget *widget, gpointer data) {
    CrasherGUI *gui = (CrasherGUI*)data;
    
    const char *hash = gtk_entry_get_text(GTK_ENTRY(gui->hash_entry));
    if (strlen(hash) == 0) {
        append_result_text(gui, "❌ Error: Please enter a hash");
        return;
    }
    
    gui->current_config.hash_type = detect_hash_type(hash);
    if (gui->current_config.hash_type == HASH_UNKNOWN) {
        append_result_text(gui, "❌ Error: Invalid hash format");
        return;
    }
    
    strcpy(gui->current_hash, hash);
    gui->current_config.target_type = gtk_combo_box_get_active(GTK_COMBO_BOX(gui->target_combo)) + 1;
    gui->current_config.attack_method = gtk_combo_box_get_active(GTK_COMBO_BOX(gui->method_combo)) + 1;
    gui->current_config.max_length = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui->max_length_spin));
    gui->current_config.max_attempts = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui->max_attempts_spin));
    gui->current_config.include_special = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui->special_chars_check));
    
    const char *wordlist_dir = gtk_entry_get_text(GTK_ENTRY(gui->wordlist_entry));
    gui->current_config.wordlist_dir = safe_strdup(wordlist_dir);
    
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gui->result_label));
    gtk_text_buffer_set_text(buffer, "", 0);
    
    char info[512];
    snprintf(info, sizeof(info), "🎯 Starting %s attack on %s hash", 
             gui->current_config.attack_method == 1 ? "Dictionary" :
             gui->current_config.attack_method == 2 ? "Brute Force" :
             gui->current_config.attack_method == 3 ? "Random" : "Auto",
             hash_type_name(gui->current_config.hash_type));
    append_result_text(gui, info);
    
    gtk_widget_set_sensitive(gui->start_button, FALSE);
    gtk_widget_set_sensitive(gui->stop_button, TRUE);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gui->progress_bar), 0.0);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(gui->progress_bar), "Starting...");
    
    gui->attack_running = TRUE;
    g_stop_attack = FALSE;
    
    pthread_create(&attack_thread, NULL, attack_worker, gui);
    
    gui->progress_timer = g_timeout_add(100, update_progress, gui);
}

static void on_stop_clicked(GtkWidget *widget, gpointer data) {
    CrasherGUI *gui = (CrasherGUI*)data;
    
    g_stop_attack = TRUE;
    append_result_text(gui, "⏹️ Stopping attack...");
    
    gtk_widget_set_sensitive(gui->stop_button, FALSE);
}

static void *attack_worker(void *data) {
    CrasherGUI *gui = (CrasherGUI*)data;
    
    attack_result_t *result = attack_engine_run(gui->current_hash, &gui->current_config);
    
    gdk_threads_add_idle_full(G_PRIORITY_HIGH_IDLE, (GSourceFunc)update_progress, gui, NULL);
    
    char result_text[512];
    if (result && result->found) {
        snprintf(result_text, sizeof(result_text), 
                "🎉 SUCCESS! Password found: %s\nAttempts: %lu\nTime: %.2f seconds\nRate: %.0f/sec",
                result->password, result->attempts, result->time_taken, result->rate);
    } else {
        snprintf(result_text, sizeof(result_text),
                "❌ Attack completed - Password not found\nAttempts: %lu\nTime: %.2f seconds",
                result ? result->attempts : 0, result ? result->time_taken : 0.0);
    }
    
    gdk_threads_add_idle_full(G_PRIORITY_HIGH_IDLE, 
        (GSourceFunc)([](gpointer data) -> gboolean {
            char *text = (char*)data;
            append_result_text(g_gui, text);
            free(text);
            return G_SOURCE_REMOVE;
        }), strdup(result_text), NULL);
    
    if (result) {
        destroy_attack_result(result);
    }
    
    gui->attack_running = FALSE;
    
    return NULL;
}

static gboolean update_progress(gpointer data) {
    CrasherGUI *gui = (CrasherGUI*)data;
    
    if (!gui->attack_running) {
        gtk_widget_set_sensitive(gui->start_button, TRUE);
        gtk_widget_set_sensitive(gui->stop_button, FALSE);
        gtk_progress_bar_set_text(GTK_PROGRESS_BAR(gui->progress_bar), "Ready");
        
        if (gui->progress_timer > 0) {
            g_source_remove(gui->progress_timer);
            gui->progress_timer = 0;
        }
        
        safe_free((void**)&gui->current_config.wordlist_dir);
        
        return G_SOURCE_REMOVE;
    }
    
    static double progress = 0.0;
    progress += 0.01;
    if (progress > 1.0) progress = 0.0;
    
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(gui->progress_bar), progress);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(gui->progress_bar), "Working...");
    
    return G_SOURCE_CONTINUE;
}

void gui_cleanup(CrasherGUI *gui) {
    if (gui->attack_running) {
        g_stop_attack = TRUE;
        pthread_join(attack_thread, NULL);
    }
    
    if (gui->progress_timer > 0) {
        g_source_remove(gui->progress_timer);
    }
    
    safe_free((void**)&gui->current_config.wordlist_dir);
}

int gui_main(int argc, char *argv[]) {
    CrasherGUI gui = {0};
    g_gui = &gui;
    
    gui_init(&gui);
    gtk_main();
    gui_cleanup(&gui);
    
    return 0;
}

#else

int gui_main(void) {
    printf("GUI not available - GTK+ not installed.\n");
    printf("Use --cli for command-line interface.\n");
    return 1;
}

#endif
