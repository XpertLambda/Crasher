#include "../../Include.h"
#include <omp.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>


volatile sig_atomic_t found = 0;

void index_to_string(uint64_t index, char *str, int length, char start, char end) {
    int num_chars = end - start + 1;
    for (int pos = length - 1; pos >= 0; pos--) {
        str[pos] = start + (index % num_chars);
        index /= num_chars;
    }
}

void process_combinations(int combination_length, char start_char, char end_char, const char* key, int type) {
    int num_chars = end_char - start_char + 1;
    uint64_t total_combinations = 1;

    for (int i = 0; i < combination_length; i++) {
        uint64_t prev = total_combinations;
        total_combinations *= num_chars;
        if (total_combinations / num_chars != prev) {
            printf("\nOverflow at length %d, skipping.", combination_length);
            return;
        }
    }

    char *key_copy = strdup(key);
    #pragma omp parallel shared(found) firstprivate(combination_length, start_char, end_char, key_copy, type, num_chars)
    {
        char *prefix = malloc(combination_length + 1);
        prefix[combination_length] = '\0';

        #pragma omp for schedule(dynamic)
        for (uint64_t i = 0; i < total_combinations; i++) {
            if (found) continue;

            index_to_string(i, prefix, combination_length, start_char, end_char);

            int result = 0;
            switch(type) {
                case 1: 
                    result = MD5Hash(prefix, key_copy);
                    break;
                case 2: 
                    result = SHA1Hash(prefix, key_copy);
                    break;    
                case 3: 
                    result = SHA256Hash(prefix, key_copy);
                    break;
                default: 
                    result = 0;
            }

            if (result == 1) {
                #pragma omp critical
                {
                    if (!found) {
                        printf("\nPassword cracked: %s", prefix);
                        found = 1;
                    }
                }
            }
        }
        free(prefix);
    }
    free(key_copy);
}

void Output(int* length, int* Op, char* s, char* e) {
    char* mode;
    switch(Op[0]) {
        case 1:
            mode = "PassWord";
            *s = 33;
            *e = 126;
            break;
        case 2:
            mode = "PinCode";
            *s = 48;
            *e = 57;
            break;
        default:
            mode = "???";
    }
    printf("\n--------------ASCII INCREMENT MODE: %s--------------", mode);
    int l = -1;
    while (l < 0) {
        printf("\nEnter the maximum length\n--> ");
        scanf("%d", &l);
    }
    *length = l;
}

int Increment(const char* key, int type, int* Op) {
    char start_char = 48;
    char end_char = 57;
    int length = 10;
    Output(&length, Op, &start_char, &end_char);

    for (int combination_length = 1; combination_length <= length; combination_length++) {
        if (found) break;
        printf("\nTrying With Length %d...", combination_length);
        fflush(stdout);

        process_combinations(combination_length, start_char, end_char, key, type);

        if (found) {
            exit(0);
        }
    }
    return 0;
}