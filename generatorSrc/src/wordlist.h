#ifndef WORDLIST_H
#define WORDLIST_H

int generate_custom_wordlist(const char *filename, const char *keywords, int minLen, int maxLen, int includeSpecial);
int generate_passcode_wordlist(const char *filename, int minLen, int maxLen);
int generate_random_wordlist(const char *filename, int listSize, int wordLength, int includeSpecial);

#endif /* WORDLIST_H */