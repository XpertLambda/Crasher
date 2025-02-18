#include "Include.h"

void Option(int* Op){
	int i=0;
	while(i==0){
		printf("\n[1] Password ?\n[2] Pincode ?\n==> ");
		scanf("%d",&Op[i]);
		if(Op[i]==1 || Op[i]==2) i++;
	}
		while(i==1){
		printf("\n[1] WordList ?\n[2] ASCII increment ? (Very Slow)\n[3] Random Generate ?\n[4] Auto\n==> ");
		scanf("%d",&Op[i]);
		if(Op[i]<=4 && Op[i]>=1) i++;
		}
		
	//system("clear");
}
int HashType(const char *hash) {
    size_t hash_length = strlen(hash);

    if (hash_length == MD5_DIGEST_LENGTH * 2) {
        printf("MD5 hash detected\n");
	return 1;
    } else if (hash_length == SHA_DIGEST_LENGTH * 2) {
        printf("SHA-1 hash detected\n");
	return 2;
    } else if (hash_length == SHA256_DIGEST_LENGTH * 2) {
        printf("SHA-256 hash detected\n");
    } else {
	   printf("Unknown hash type\n");
	   exit(0);
	return 3;
    }
}

int main(void){
	system("clear");
	Logo();
	printf("This Project About De-Hashing");
	printf("\nSupported Hash Types ==> MD5 , SHA1, SHA256");
	
	char key[50];
     	printf("\nEnter the hash : ");
	scanf("%49s", key);
	
	printf("==>");
	int type = HashType(key);
	int option[2];
	Option(option);
	DeHash(key, type,option);
	

}
