#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <time.h>

double get_time_diff(clock_t start, clock_t end);
void print_progress(uint64_t current, uint64_t total, double rate);
void print_success(const char *password, uint64_t attempts, double time_taken);
void print_failure(const char *method, uint64_t attempts, double time_taken);

hash_type_t detect_hash_type(const char *hash);
const char* hash_type_name(hash_type_t type);

char* build_charset(target_type_t target, bool include_special);
int get_charset_size(target_type_t target, bool include_special);

void* safe_malloc(size_t size);
char* safe_strdup(const char *str);
void safe_free(void **ptr);

bool is_valid_hex(const char *str, size_t expected_len);
void to_lowercase(char *str);

#endif
