#include "attack_engine.h"
#include "../crypto/hash_engine.h"
#include "../common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <omp.h>
#include <sys/stat.h>

#ifndef DT_REG
#define DT_REG 8
#endif

// Global stop flag for coordinated stopping across threads
static volatile bool g_stop_attack = false;

attack_result_t* create_attack_result(void) {
    attack_result_t *result = safe_malloc(sizeof(attack_result_t));
    memset(result, 0, sizeof(attack_result_t));
    return result;
}

void destroy_attack_result(attack_result_t *result) {
    if (result) {
        safe_free((void**)&result->password);
        free(result);
    }
}

attack_result_t* attack_engine_run(const char *hash, const attack_config_t *config) {
    if (!hash || !config) {
        return NULL;
    }
    
    g_stop_attack = false;
    
    switch (config->attack_method) {
        case ATTACK_DICTIONARY:
            return dictionary_attack(hash, config);
        case ATTACK_BRUTE_FORCE:
            return brute_force_attack(hash, config);
        case ATTACK_RANDOM:
            return random_attack(hash, config);
        case ATTACK_AUTO:
            return auto_attack(hash, config);
        default:
            return NULL;
    }
}

attack_result_t* dictionary_attack(const char *hash, const attack_config_t *config) {
    printf("=== DICTIONARY ATTACK ===\n");
    printf("Target: %s hash\n", hash_type_name(config->hash_type));
    printf("Dictionary: %s\n", config->wordlist_dir ? config->wordlist_dir : "current directory");
    
    attack_result_t *result = create_attack_result();
    clock_t start_time = clock();
    
    hash_verify_func_t verify = get_hash_verifier(config->hash_type);
    if (!verify) {
        printf("Error: Unsupported hash type\n");
        return result;
    }
    
    const char *dir_path = config->wordlist_dir ? config->wordlist_dir : ".";
    DIR *dp = opendir(dir_path);
    
    if (!dp) {
        printf("Error: Cannot open directory '%s'\n", dir_path);
        return result;
    }
    
    struct dirent *entry;
    int files_processed = 0;
    
    while ((entry = readdir(dp)) && !g_stop_attack) {
        if (entry->d_type != DT_REG) continue;
        
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);
        
        FILE *fp = fopen(file_path, "r");
        if (!fp) continue;
        
        printf("Processing: %s\n", entry->d_name);
        files_processed++;
        
        char line[256];
        while (fgets(line, sizeof(line), fp) && !g_stop_attack) {
            // Remove trailing newline/carriage return
            line[strcspn(line, "\r\n")] = '\0';
            
            if (strlen(line) == 0) continue;
            
            result->attempts++;
            
            if (result->attempts % 10000 == 0) {
                double elapsed = get_time_diff(start_time, clock());
                double rate = elapsed > 0 ? result->attempts / elapsed : 0;
                print_progress(result->attempts, 0, rate);
            }
            
            if (verify(line, hash)) {
                result->found = true;
                result->password = safe_strdup(line);
                result->time_taken = get_time_diff(start_time, clock());
                result->rate = result->time_taken > 0 ? result->attempts / result->time_taken : 0;
                
                fclose(fp);
                closedir(dp);
                print_success(result->password, result->attempts, result->time_taken);
                return result;
            }
        }
        
        fclose(fp);
    }
    
    closedir(dp);
    
    result->time_taken = get_time_diff(start_time, clock());
    result->rate = result->time_taken > 0 ? result->attempts / result->time_taken : 0;
    
    printf("\nDictionary attack completed.\n");
    printf("Files processed: %d\n", files_processed);
    print_failure("Dictionary", result->attempts, result->time_taken);
    
    return result;
}

// Optimized index to string conversion with lookup table
static void index_to_string_fast(uint64_t index, char *str, int length, const char *charset, int charset_size) {
    for (int pos = length - 1; pos >= 0; pos--) {
        str[pos] = charset[index % charset_size];
        index /= charset_size;
    }
    str[length] = '\0';
}

attack_result_t* brute_force_attack(const char *hash, const attack_config_t *config) {
    printf("=== BRUTE FORCE ATTACK ===\n");
    printf("Target: %s hash\n", hash_type_name(config->hash_type));
    printf("Type: %s\n", config->target_type == TARGET_PIN ? "PIN" : "Password");
    printf("Max length: %d\n", config->max_length);
    
    attack_result_t *result = create_attack_result();
    clock_t start_time = clock();
    
    char *charset = build_charset(config->target_type, config->include_special);
    int charset_size = strlen(charset);
    
    printf("Character set size: %d\n", charset_size);
    printf("Starting brute force...\n");

    // Sequential brute force to avoid OpenMP memory issues
    hash_context_t *hash_ctx = hash_context_create(config->hash_type);
    if (!hash_ctx) {
        printf("Error: Failed to create hash context\n");
        free(charset);
        return result;
    }

    for (int length = 1; length <= config->max_length && !g_stop_attack && !result->found; length++) {
        uint64_t total_combinations = 1;
        
        // Calculate total combinations for this length
        for (int i = 0; i < length; i++) {
            uint64_t prev = total_combinations;
            total_combinations *= charset_size;
            if (total_combinations / charset_size != prev) {
                printf("Overflow at length %d\n", length);
                break;
            }
        }
        
        printf("Testing length %d (%lu combinations)...\n", length, total_combinations);
        
        char *candidate = safe_malloc(config->max_length + 1);
        if (!candidate) {
            printf("Error: Failed to allocate candidate buffer\n");
            break;
        }
        
        for (uint64_t i = 0; i < total_combinations && !g_stop_attack && !result->found; i++) {
            index_to_string_fast(i, candidate, length, charset, charset_size);
            result->attempts++;
            
            if (hash_context_verify(hash_ctx, candidate, hash)) {
                result->found = true;
                result->password = safe_strdup(candidate);
                result->time_taken = get_time_diff(start_time, clock());
                result->rate = result->time_taken > 0 ? result->attempts / result->time_taken : 0;
                break;
            }
            
            // Progress reporting
            if (result->attempts % 50000 == 0) {
                double elapsed = get_time_diff(start_time, clock());
                double rate = elapsed > 0 ? result->attempts / elapsed : 0;
                print_progress(result->attempts, 0, rate);
            }
        }
        
        free(candidate);
        if (result->found) break;
    }
    
    hash_context_destroy(hash_ctx);
    free(charset);
    
    if (!result->time_taken) {
        result->time_taken = get_time_diff(start_time, clock());
        result->rate = result->time_taken > 0 ? result->attempts / result->time_taken : 0;
    }
    
    if (result->found) {
        print_success(result->password, result->attempts, result->time_taken);
    } else {
        print_failure("Brute force", result->attempts, result->time_taken);
    }
    
    return result;
}

attack_result_t* random_attack(const char *hash, const attack_config_t *config) {
    printf("=== RANDOM ATTACK ===\n");
    printf("Target: %s hash\n", hash_type_name(config->hash_type));
    printf("Max attempts: %lu\n", config->max_attempts);
    
    attack_result_t *result = create_attack_result();
    clock_t start_time = clock();
    
    char *charset = build_charset(config->target_type, config->include_special);
    int charset_size = strlen(charset);
    
    hash_verify_func_t verify = get_hash_verifier(config->hash_type);
    if (!verify) {
        printf("Error: Unsupported hash type\n");
        free(charset);
        return result;
    }
    
    srand(time(NULL));
    
    char *candidate = safe_malloc(config->max_length + 1);
    
    printf("Starting random attack...\n");
    
    while (result->attempts < config->max_attempts && !g_stop_attack) {
        int length = 1 + rand() % config->max_length;
        
        for (int i = 0; i < length; i++) {
            candidate[i] = charset[rand() % charset_size];
        }
        candidate[length] = '\0';
        
        result->attempts++;
        
        if (result->attempts % 50000 == 0) {
            double elapsed = get_time_diff(start_time, clock());
            double rate = elapsed > 0 ? result->attempts / elapsed : 0;
            print_progress(result->attempts, config->max_attempts, rate);
        }
        
        if (verify(candidate, hash)) {
            result->found = true;
            result->password = safe_strdup(candidate);
            break;
        }
    }
    
    free(candidate);
    free(charset);
    
    result->time_taken = get_time_diff(start_time, clock());
    result->rate = result->time_taken > 0 ? result->attempts / result->time_taken : 0;
    
    if (result->found) {
        print_success(result->password, result->attempts, result->time_taken);
    } else {
        print_failure("Random", result->attempts, result->time_taken);
    }
    
    return result;
}

attack_result_t* auto_attack(const char *hash, const attack_config_t *config) {
    printf("=== AUTO ATTACK MODE ===\n");
    printf("Will try: Dictionary → Random → Brute Force\n\n");
    
    attack_config_t temp_config = *config;
    attack_result_t *result;
    
    // Phase 1: Dictionary attack
    printf("Phase 1: Dictionary Attack\n");
    temp_config.attack_method = ATTACK_DICTIONARY;
    result = dictionary_attack(hash, &temp_config);
    if (result->found) return result;
    destroy_attack_result(result);
    
    // Phase 2: Random attack (limited)
    printf("\nPhase 2: Random Attack\n");
    temp_config.attack_method = ATTACK_RANDOM;
    temp_config.max_attempts = 1000000; // Limit random attempts
    result = random_attack(hash, &temp_config);
    if (result->found) return result;
    destroy_attack_result(result);
    
    // Phase 3: Brute force (limited length)
    printf("\nPhase 3: Brute Force Attack\n");
    temp_config.attack_method = ATTACK_BRUTE_FORCE;
    temp_config.max_length = (config->target_type == TARGET_PIN) ? 8 : 6; // Reasonable limits
    result = brute_force_attack(hash, &temp_config);
    
    return result;
}
