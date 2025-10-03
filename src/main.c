#include "ui/ui.h"
#include "gui/gui.h"
#include "engine/attack_engine.h"
#include "common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static volatile bool g_running = true;

void signal_handler(int sig) {
    (void)sig;
    printf("\nReceived interrupt signal. Stopping...\n");
    g_running = false;
}

void show_usage(const char *program_name) {
    printf("Crasher - Professional Hash Cracking Tool\n");
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Options:\n");
    printf("  --gui     Launch graphical interface\n");
    printf("  --cli     Launch command-line interface (default)\n");
    printf("  --help    Show this help message\n");
    printf("  --version Show version information\n");
}

void show_version(void) {
    printf("Crasher Professional v2.0\n");
    printf("Advanced Hash Cracking Toolkit\n");
    printf("Supports: MD5, SHA-1, SHA-256\n");
    printf("Built with: OpenSSL, OpenMP, GTK+\n");
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    bool use_gui = false;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--gui") == 0) {
            use_gui = true;
        } else if (strcmp(argv[i], "--cli") == 0) {
            use_gui = false;
        } else if (strcmp(argv[i], "--help") == 0) {
            show_usage(argv[0]);
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "--version") == 0) {
            show_version();
            return EXIT_SUCCESS;
        } else {
            printf("Unknown option: %s\n", argv[i]);
            show_usage(argv[0]);
            return EXIT_FAILURE;
        }
    }
    
    if (use_gui) {
        return gui_main();
    } else {
        return cli_main();
    }
}
