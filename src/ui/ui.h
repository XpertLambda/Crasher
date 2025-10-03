#ifndef UI_H
#define UI_H

#include <stddef.h>
#include <stdbool.h>
#include "../common/types.h"

int cli_main(void);
void show_banner(void);
void show_help(void);
hash_type_t get_hash_input(char *hash_buffer, size_t buffer_size);
void get_attack_config(attack_config_t *config);
void show_attack_summary(const attack_config_t *config);

bool validate_hash_input(const char *hash);
int get_int_input(const char *prompt, int min_val, int max_val);
char* get_string_input(const char *prompt, size_t max_length);

#endif
