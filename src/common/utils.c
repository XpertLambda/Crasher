#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char DIGITS[] = "0123456789";
static const char LOWERCASE[] = "abcdefghijklmnopqrstuvwxyz";
static const char UPPERCASE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char SPECIAL[] = "!@#$%^&*()-=_+[]{}|;:'\",.<>?/\\`~";

double get_time_diff(clock_t start, clock_t end) {
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

void print_progress(uint64_t current, uint64_t total, double rate) {
    if (total > 0) {
        double percent = ((double)current / total) * 100.0;
        printf("\rProgress: %.2f%% (%lu/%lu) | Rate: %.0f/sec", 
               percent, current, total, rate);
    } else {
        printf("\rAttempts: %lu | Rate: %.0f/sec", current, rate);
    }
    fflush(stdout);
}

void print_success(const char *password, uint64_t attempts, double time_taken) {
    printf("\nPassword found: %s\n", password);
    printf("Attempts: %lu\n", attempts);
    printf("Time: %.2f seconds\n", time_taken);
    printf("Rate: %.0f attempts/second\n", attempts / time_taken);
}

void print_failure(const char *method, uint64_t attempts, double time_taken) {
    printf("\n%s attack completed.\n", method);
    printf("Attempts: %lu\n", attempts);
    printf("Time: %.2f seconds\n", time_taken);
    if (time_taken > 0) {
        printf("Average rate: %.0f attempts/second\n", attempts / time_taken);
    }
    printf("Password not found.\n");
}

hash_type_t detect_hash_type(const char *hash) {
    if (!hash) return HASH_UNKNOWN;
    
    size_t len = strlen(hash);
    
    if (!is_valid_hex(hash, len)) {
        return HASH_UNKNOWN;
    }
    
    switch (len) {
        case 32:  return HASH_MD5;
        case 40:  return HASH_SHA1;
        case 64:  return HASH_SHA256;
        default:  return HASH_UNKNOWN;
    }
}

const char* hash_type_name(hash_type_t type) {
    switch (type) {
        case HASH_MD5:    return "MD5";
        case HASH_SHA1:   return "SHA-1";
        case HASH_SHA256: return "SHA-256";
        default:          return "Unknown";
    }
}

char* build_charset(target_type_t target, bool include_special) {
    size_t total_size = strlen(DIGITS) + strlen(LOWERCASE) + strlen(UPPERCASE) + 
                       (include_special ? strlen(SPECIAL) : 0) + 1;
    
    char *charset = safe_malloc(total_size);
    charset[0] = '\0';
    
    switch (target) {
        case TARGET_PIN:
            strcpy(charset, DIGITS);
            break;
        case TARGET_PASSWORD:
            strcpy(charset, DIGITS);
            strcat(charset, LOWERCASE);
            strcat(charset, UPPERCASE);
            if (include_special) {
                strcat(charset, SPECIAL);
            }
            break;
    }
    
    return charset;
}

int get_charset_size(target_type_t target, bool include_special) {
    switch (target) {
        case TARGET_PIN:
            return strlen(DIGITS);
        case TARGET_PASSWORD:
            return strlen(DIGITS) + strlen(LOWERCASE) + strlen(UPPERCASE) + 
                   (include_special ? strlen(SPECIAL) : 0);
        default:
            return 0;
    }
}

void* safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

char* safe_strdup(const char *str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    char *copy = safe_malloc(len + 1);
    strcpy(copy, str);
    return copy;
}

void safe_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

bool is_valid_hex(const char *str, size_t expected_len) {
    if (!str || strlen(str) != expected_len) {
        return false;
    }
    
    for (size_t i = 0; i < expected_len; i++) {
        if (!isxdigit(str[i])) {
            return false;
        }
    }
    return true;
}

void to_lowercase(char *str) {
    if (!str) return;
    
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}
