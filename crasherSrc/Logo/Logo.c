//out.c
#include "../Include.h"
void Logo(){ 
    FILE *fptr; 
    char* filename="Logo/logo.txt" , c;
    // Open file 
    fptr = fopen(filename, "r"); 
    if (fptr == NULL) 
    { 
        printf("Cannot open Logo file \n"); 
        exit(0); 
    } 
  
    // Read contents from file 
    c = fgetc(fptr); 
    while (c != EOF) 
    { 
        printf ("%c", c); 
        c = fgetc(fptr); 
    } 
    fclose(fptr);  
}


