//Options
#include "../../Include.h"

int Random(const char* key,int type, int *Op){
	printf("\n---------------Random Generator---------------");
	printf("\nEnter The Length (0 for Unknown) : ");
	int inp = 0, length = 20;
	do{
	printf("\n--> ");
	scanf("%d",&inp);
	}while(inp<0);
	if(inp!=0) length = inp;
	int s,e,result=0;
	long int count=0;
	char* word = malloc(sizeof(char)*length);

	switch(Op[0]){
		case 1 : 
		s = 33;
		e = 126;
		break;
		case 2:
		s=48;
		e=57;
		break;
		default:
		printf("\nError Op[] switch");
		exit(0);
	}

    for (long int i = 0; i >= 0; i++) {
	    if(inp==0) length = 1 + rand()%15;
        for (int j = 0; j < length ; j++) {
             word[j] =  s + rand() % (e - s + 1); 
        }
	word[length]= '\0';
       switch (type) {
                    case 1:
                        result = MD5Hash(word, key);
                        break;
                    case 2:
                        result = SHA1Hash(word, key);
                        break;
                    case 3:
                        result = SHA256Hash(word, key);
                        break;
                    default:
			printf("Unkown Type");
                        exit(0);
                }
       count++;
       if(count%300000 == 0){ 
	       printf("\nGenerated %ld Password (%s)",count,word);
       }
       if(result){
       printf("\nPassword Cracked : %s\n==> After %ld tries",word,count);
       exit(0);
       }

    }

   
    return 0;}
