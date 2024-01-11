#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLEN 100

char str1[MAXLEN];
char str2[MAXLEN];

int main(){
	
	
	printf("Enter String #1:");
	fgets(str1, MAXLEN, stdin);
	printf("Enter String #2:");
	fgets(str2, MAXLEN, stdin);

	//puts(str1);
	//puts(str2);

	int same = 1;

	char* p1 = str1;
	char* p2 = str2;

	for(int i=0; i<MAXLEN; i++){
		while(*p1 == ' '){
			p1++;
		}
		while(*p2 == ' '){
			p2++;
		}
		if(toupper(*p1) != toupper(*p2)){
			same = 0;
			break;
		}
		p1++;
		p2++;

	}
	
	if(same){
		puts("Both strings are same!");
	} else {
		puts("Both strings are different!");
	}

	return 0;

}
