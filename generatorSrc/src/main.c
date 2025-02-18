#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordlist.h"

#define MAX_INPUT_LENGTH 128

int main(void) {
    char filename[MAX_INPUT_LENGTH];
    int modeSelection;
    int subModeSelection;
    int minLen, maxLen;
    char keywords[MAX_INPUT_LENGTH];
    int includeSpecial = 0;

    /* 1. Initial File Name Prompt */
    printf("Enter the desired name for the wordlist file (e.g., wordlist.txt): ");
    if (scanf("%127s", filename) != 1) {
        fprintf(stderr, "Invalid input for filename.\n");
        return 1;
    }

    /* 2. Mode Selection */
    printf("Select a mode:\n");
    printf("1) Customized\n");
    printf("2) Random\n");
    printf("Enter choice (1 or 2): ");
    if (scanf("%d", &modeSelection) != 1) {
        fprintf(stderr, "Invalid selection.\n");
        return 1;
    }

    if (modeSelection == 1) {
        /* Customized Mode */
        printf("Choose a sub-option:\n");
        printf("1) Passchar\n");
        printf("2) Passcode\n");
        printf("3) Unknown\n");
        printf("Enter choice (1, 2, or 3): ");
        if (scanf("%d", &subModeSelection) != 1) {
            fprintf(stderr, "Invalid sub-option selection.\n");
            return 1;
        }

        switch (subModeSelection) {
            case 1: /* Passchar */
            case 3: /* Unknown */
                {
                    /* Prompt for keywords */
                    printf("Enter keywords (leave empty for Unknown if desired): ");
                    getchar(); /* consume leftover newline */
                    fgets(keywords, sizeof(keywords), stdin);

                    /* Remove trailing newline in keywords if any */
                    char *newlinePos = strchr(keywords, '\n');
                    if (newlinePos) {
                        *newlinePos = '\0';
                    }

                    /* Prompt for lengths */
                    printf("Enter minimum length: ");
                    if (scanf("%d", &minLen) != 1) {
                        fprintf(stderr, "Invalid input.\n");
                        return 1;
                    }
                    printf("Enter maximum length: ");
                    if (scanf("%d", &maxLen) != 1) {
                        fprintf(stderr, "Invalid input.\n");
                        return 1;
                    }
                    if (minLen < 1 || maxLen < minLen) {
                        fprintf(stderr, "Invalid length range.\n");
                        return 1;
                    }

                    /* Ask about special characters */
                    printf("Include special characters? (1 = Yes, 0 = No): ");
                    if (scanf("%d", &includeSpecial) != 1) {
                        fprintf(stderr, "Invalid input.\n");
                        return 1;
                    }

                    /* Generate wordlist */
                    if (generate_custom_wordlist(filename, keywords, minLen, maxLen, includeSpecial) != 0) {
                        fprintf(stderr, "Wordlist generation failed.\n");
                        return 1;
                    }
                }
                break;
            case 2: /* Passcode */
                {
                    /* Prompt for passcode lengths */
                    printf("Enter minimum passcode length: ");
                    if (scanf("%d", &minLen) != 1) {
                        fprintf(stderr, "Invalid input.\n");
                        return 1;
                    }
                    printf("Enter maximum passcode length: ");
                    if (scanf("%d", &maxLen) != 1) {
                        fprintf(stderr, "Invalid input.\n");
                        return 1;
                    }
                    if (minLen < 1 || maxLen < minLen) {
                        fprintf(stderr, "Invalid length range.\n");
                        return 1;
                    }
                    /* Generate passcode wordlist */
                    if (generate_passcode_wordlist(filename, minLen, maxLen) != 0) {
                        fprintf(stderr, "Wordlist generation failed.\n");
                        return 1;
                    }
                }
                break;
            default:
                fprintf(stderr, "Unknown sub-option.\n");
                return 1;
        }
    } else if (modeSelection == 2) {
        /* Random Mode */
        int listSize;
        int wordLength;
        printf("Enter wordlist size (number of random words): ");
        if (scanf("%d", &listSize) != 1 || listSize <= 0) {
            fprintf(stderr, "Invalid list size.\n");
            return 1;
        }
        printf("Enter length of each random word: ");
        if (scanf("%d", &wordLength) != 1 || wordLength <= 0) {
            fprintf(stderr, "Invalid word length.\n");
            return 1;
        }
        printf("Include special characters? (1 = Yes, 0 = No): ");
        if (scanf("%d", &includeSpecial) != 1) {
            fprintf(stderr, "Invalid input.\n");
            return 1;
        }

        if (generate_random_wordlist(filename, listSize, wordLength, includeSpecial) != 0) {
            fprintf(stderr, "Wordlist generation failed.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Invalid mode selection.\n");
        return 1;
    }

    printf("Wordlist generation complete. Output saved to %s\n", filename);
    return 0;
}