#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wordlist.h"

/* Basic character sets */
static const char *ALPHANUM = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static const char *SPECIALS = "!@#$%^&*()-=_+[]{}|;:'\",.<>?/\\`~";

/* Helper function to write a generated word to file */
static int write_word(FILE *fp, const char *word) {
    if (fprintf(fp, "%s\n", word) < 0) {
        return -1;
    }
    return 0;
}

int generate_combinations(FILE *fp, const char *charset, char *buffer, int index, int length) {
    if (index == length) {
        buffer[length] = '\0';
        fprintf(fp, "%s\n", buffer);
        return 0;
    }

    for (size_t i = 0; i < strlen(charset); i++) {
        buffer[index] = charset[i];
        if (generate_combinations(fp, charset, buffer, index + 1, length) != 0) {
            return -1;
        }
    }

    return 0;
}

int generate_custom_wordlist(const char *filename, const char *keywords, int minLen, int maxLen, int includeSpecial) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    /* Build character set */
    char charset[512] = {0};
    strcpy(charset, ALPHANUM);
    if (includeSpecial) {
        strcat(charset, SPECIALS);
    }

    /*
       Optionally, we might want to augment the character set with unique
       characters from the keywords. This helps incorporate user-supplied chars.
    */
    for (size_t i = 0; i < strlen(keywords); i++) {
        if (!strchr(charset, keywords[i]) && keywords[i] != ' ' && keywords[i] != '\t') {
            size_t len = strlen(charset);
            if (len < sizeof(charset) - 1) {
                charset[len] = keywords[i];
                charset[len + 1] = '\0';
            }
        }
    }

    /* First, write the keywords to the file */
    char *keywordCopy = strdup(keywords);
    if (!keywordCopy) {
        fclose(fp);
        return -1;
    }

    char *token = strtok(keywordCopy, " \t");
    while (token != NULL) {
        if (write_word(fp, token) != 0) {
            free(keywordCopy);
            fclose(fp);
            return -1;
        }
        token = strtok(NULL, " \t");
    }
    free(keywordCopy);

    /* Generate permutations from minLen to maxLen */
    char *buffer = (char *)malloc(maxLen + 1);
    if (!buffer) {
        fclose(fp);
        return -1;
    }

    for (int length = minLen; length <= maxLen; length++) {
        if (length <= 0) {
            continue;
        }
        if (generate_combinations(fp, charset, buffer, 0, length) != 0) {
            fclose(fp);
            free(buffer);
            return -1;
        }
    }

    free(buffer);
    fclose(fp);
    return 0;
}

/* Generate passcode-style wordlist from minLen to maxLen 
   This example simply uses digits; adapt as needed. */
int generate_passcode_wordlist(const char *filename, int minLen, int maxLen) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    /* Numeric-only for passcodes, can be expanded if needed */
    const char *digits = "0123456789";

    char *buffer = (char *)malloc(maxLen + 1);
    if (!buffer) {
        fclose(fp);
        return -1;
    }

    for (int length = minLen; length <= maxLen; length++) {
        if (length <= 0) {
            continue;
        }
        if (generate_combinations(fp, digits, buffer, 0, length) != 0) {
            fclose(fp);
            free(buffer);
            return -1;
        }
    }

    free(buffer);
    fclose(fp);
    return 0;
}

/* Generate random wordlist of a fixed length for each word */
int generate_random_wordlist(const char *filename, int listSize, int wordLength, int includeSpecial) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    /* Build character set */
    char charset[512] = {0};
    strcpy(charset, ALPHANUM);
    if (includeSpecial) {
        strcat(charset, SPECIALS);
    }
    size_t charCount = strlen(charset);

    srand((unsigned int)time(NULL));

    char *wordBuffer = (char *)malloc(wordLength + 1);
    if (!wordBuffer) {
        fclose(fp);
        return -1;
    }

    for (int i = 0; i < listSize; i++) {
        for (int j = 0; j < wordLength; j++) {
            wordBuffer[j] = charset[rand() % charCount];
        }
        wordBuffer[wordLength] = '\0';
        if (write_word(fp, wordBuffer) != 0) {
            free(wordBuffer);
            fclose(fp);
            return -1;
        }
    }

    free(wordBuffer);
    fclose(fp);
    return 0;
}