#ifndef DeHash_H
#define DeHash_H

void DeHash(const char* , int , int* );

int WordList(const char* ,int );

int Increment(const char* , int , int* );
void print_combinations(int, char*, char, char,const char*, int);
void Output(int*, int*, char*, char*);

int Random(const char*, int, int*);

int MD5Hash(const char *content, const char *key);
int SHA1Hash(const char *content, const char *key);
int SHA256Hash(const char *content, const char *key);





#endif
