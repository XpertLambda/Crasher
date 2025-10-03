#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    HASH_UNKNOWN = 0,
    HASH_MD5     = 1,
    HASH_SHA1    = 2,
    HASH_SHA256  = 3
} hash_type_t;

typedef enum {
    ATTACK_DICTIONARY = 1,
    ATTACK_BRUTE_FORCE = 2,
    ATTACK_RANDOM = 3,
    ATTACK_AUTO = 4
} attack_method_t;

typedef enum {
    TARGET_PASSWORD = 1,
    TARGET_PIN = 2
} target_type_t;

typedef struct {
    hash_type_t hash_type;
    attack_method_t attack_method;
    target_type_t target_type;
    int max_length;
    char *wordlist_dir;
    char *charset;
    bool include_special;
    uint64_t max_attempts;
} attack_config_t;

typedef struct {
    bool found;
    char *password;
    uint64_t attempts;
    double time_taken;
    double rate;
} attack_result_t;

typedef bool (*hash_verify_func_t)(const char *plaintext, const char *hash);

#endif
