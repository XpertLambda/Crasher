#include "ui.h"
#include "../engine/attack_engine.h"
#include "../common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_banner(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                    CRASHER PROFESSIONAL                      ║\n");
    printf("║              Advanced Hash Cracking Toolkit                  ║\n");
    printf("║                                                               ║\n");
    printf("║  Supported: MD5, SHA-1, SHA-256                              ║\n");
    printf("║  Methods: Dictionary, Brute Force, Random, Auto              ║\n");
    printf("║                                                               ║\n");
    printf("║  FOR EDUCATIONAL AND AUTHORIZED TESTING ONLY                 ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void show_help(void) {
    printf("Usage Instructions:\n");
    printf("==================\n");
    printf("1. Enter a hash (MD5: 32 chars, SHA-1: 40 chars, SHA-256: 64 chars)\n");
    printf("2. Select target type (Password or PIN)\n");
    printf("3. Choose attack method:\n");
    printf("   • Dictionary: Uses wordlist files\n");
    printf("   • Brute Force: Tries all combinations\n");
    printf("   • Random: Generates random candidates\n");
    printf("   • Auto: Tries all methods intelligently\n");
    printf("4. Configure attack parameters\n");
    printf("5. Monitor progress and results\n\n");
}

hash_type_t get_hash_input(char *hash_buffer, size_t buffer_size) {
    printf("Enter the hash to crack: ");
    if (!fgets(hash_buffer, buffer_size, stdin)) {
        printf("Error reading input.\n");
        return HASH_UNKNOWN;
    }
    
    hash_buffer[strcspn(hash_buffer, "\r\n")] = '\0';
    
    if (strlen(hash_buffer) == 0) {
        printf("Error: Empty hash provided.\n");
        return HASH_UNKNOWN;
    }
    
    hash_type_t type = detect_hash_type(hash_buffer);
    
    if (type == HASH_UNKNOWN) {
        printf("Error: Unknown hash type. Supported lengths:\n");
        printf("  MD5: 32 characters\n");
        printf("  SHA-1: 40 characters\n");
        printf("  SHA-256: 64 characters\n");
        printf("Current hash length: %zu\n", strlen(hash_buffer));
        return HASH_UNKNOWN;
    }
    
    printf("Detected: %s hash\n", hash_type_name(type));
    return type;
}

void get_attack_config(attack_config_t *config) {
    printf("\nAttack Configuration:\n");
    printf("====================\n");
    
    printf("Select target type:\n");
    printf("1) Password (letters, numbers, symbols)\n");
    printf("2) PIN (numbers only)\n");
    config->target_type = get_int_input("Choice", 1, 2);
    
    printf("\nSelect attack method:\n");
    printf("1) Dictionary attack (wordlist files)\n");
    printf("2) Brute force attack (all combinations)\n");
    printf("3) Random attack (random generation)\n");
    printf("4) Auto attack (intelligent sequence)\n");
    config->attack_method = get_int_input("Choice", 1, 4);
    
    switch (config->attack_method) {
        case ATTACK_DICTIONARY:
            printf("\nWordlist directory (press Enter for current directory): ");
            config->wordlist_dir = get_string_input("", 1024);
            if (strlen(config->wordlist_dir) == 0) {
                safe_free((void**)&config->wordlist_dir);
                config->wordlist_dir = safe_strdup(".");
            }
            break;
            
        case ATTACK_BRUTE_FORCE:
            printf("\nBrute force parameters:\n");
            config->max_length = get_int_input("Maximum length to try", 1, 12);
            if (config->target_type == TARGET_PASSWORD) {
                printf("Include special characters? (1=Yes, 0=No): ");
                config->include_special = get_int_input("", 0, 1);
            }
            break;
            
        case ATTACK_RANDOM:
            printf("\nRandom attack parameters:\n");
            config->max_length = get_int_input("Maximum password length", 1, 15);
            config->max_attempts = get_int_input("Maximum attempts (1-10000000)", 1, 10000000);
            if (config->target_type == TARGET_PASSWORD) {
                printf("Include special characters? (1=Yes, 0=No): ");
                config->include_special = get_int_input("", 0, 1);
            }
            break;
            
        case ATTACK_AUTO:
            printf("\nAuto mode will use intelligent defaults.\n");
            config->max_length = (config->target_type == TARGET_PIN) ? 8 : 6;
            config->max_attempts = 1000000;
            config->include_special = (config->target_type == TARGET_PASSWORD);
            config->wordlist_dir = safe_strdup(".");
            break;
    }
}

void show_attack_summary(const attack_config_t *config) {
    printf("\nAttack Summary:\n");
    printf("===============\n");
    printf("Hash type: %s\n", hash_type_name(config->hash_type));
    printf("Target: %s\n", config->target_type == TARGET_PIN ? "PIN" : "Password");
    
    switch (config->attack_method) {
        case ATTACK_DICTIONARY:
            printf("Method: Dictionary attack\n");
            printf("Wordlist directory: %s\n", config->wordlist_dir);
            break;
        case ATTACK_BRUTE_FORCE:
            printf("Method: Brute force attack\n");
            printf("Max length: %d\n", config->max_length);
            printf("Special chars: %s\n", config->include_special ? "Yes" : "No");
            break;
        case ATTACK_RANDOM:
            printf("Method: Random attack\n");
            printf("Max length: %d\n", config->max_length);
            printf("Max attempts: %lu\n", config->max_attempts);
            printf("Special chars: %s\n", config->include_special ? "Yes" : "No");
            break;
        case ATTACK_AUTO:
            printf("Method: Auto attack (all methods)\n");
            break;
    }
    
    printf("\nPress Enter to start attack...");
    getchar();
    printf("\n");
}

int get_int_input(const char *prompt, int min_val, int max_val) {
    int value;
    while (1) {
        printf("%s (%d-%d): ", prompt, min_val, max_val);
        if (scanf("%d", &value) == 1 && value >= min_val && value <= max_val) {
            while (getchar() != '\n');
            return value;
        }
        printf("Invalid input. Please enter a number between %d and %d.\n", min_val, max_val);
        while (getchar() != '\n');
    }
}

char* get_string_input(const char *prompt, size_t max_length) {
    printf("%s", prompt);
    
    char *buffer = safe_malloc(max_length + 1);
    if (!fgets(buffer, max_length + 1, stdin)) {
        buffer[0] = '\0';
    } else {
        buffer[strcspn(buffer, "\r\n")] = '\0';
    }
    
    return buffer;
}

bool validate_hash_input(const char *hash) {
    if (!hash) return false;
    
    size_t len = strlen(hash);
    return (len == 32 || len == 40 || len == 64) && is_valid_hex(hash, len);
}

void cleanup_config(attack_config_t *config) {
    if (config) {
        safe_free((void**)&config->wordlist_dir);
        safe_free((void**)&config->charset);
    }
}

int cli_main(void) {
    show_banner();
    
    char hash[256];
    attack_config_t config = {0};
    
    config.hash_type = get_hash_input(hash, sizeof(hash));
    if (config.hash_type == HASH_UNKNOWN) {
        printf("\nUse 'crasher --help' for usage instructions.\n");
        return EXIT_FAILURE;
    }
    
    get_attack_config(&config);
    show_attack_summary(&config);
    
    printf("Starting attack...\n\n");
    
    attack_result_t *result = attack_engine_run(hash, &config);
    
    if (result) {
        printf("\n═══════════════════════════════════════\n");
        if (result->found) {
            printf("SUCCESS! Password cracked!\n");
            printf("Password: %s\n", result->password);
        } else {
            printf("Attack completed - Password not found\n");
        }
        
        printf("Attempts: %lu\n", result->attempts);
        printf("Time: %.2f seconds\n", result->time_taken);
        if (result->time_taken > 0) {
            printf("Rate: %.0f attempts/second\n", result->rate);
        }
        printf("═══════════════════════════════════════\n");
        
        destroy_attack_result(result);
    } else {
        printf("Error: Attack engine failed to start.\n");
        cleanup_config(&config);
        return EXIT_FAILURE;
    }
    
    cleanup_config(&config);
    return result && result->found ? EXIT_SUCCESS : EXIT_FAILURE;
}
