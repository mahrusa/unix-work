#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXLEN 1000000
char input[MAXLEN];

int main() {
	scanf("%s", input);

	//CODE HERE!
	//
	int frontCtr = 0;
	int total = 0;

	for(int i=0; i<MAXLEN; i++){
		if(input[i] == '(' && input[i+1] == '('){
			frontCtr++;
		} else if(input[i] == ')' && input[i+1] == ')'){
			total += frontCtr;
		}
	
	}
	
	printf("%d \n", total);
}
