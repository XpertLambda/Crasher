//DeHash//
#include "../Include.h"

void DeHash(const char* key, int type, int* Op){
	switch(Op[1]){
		case 1:
			WordList(key, type);
			break;
		case 2:
			Increment(key, type,Op);
			break;
		case 3: 
			Random(key, type, Op);
			break;
	}
}
