#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXLEN 1000000
#define MAXLEGS 500000

char input[MAXLEN];

int main() {
	int numFrontLegs[MAXLEGS];
	int numBackLegs[MAXLEGS];
	int frontCtr = 0;
	int backCtr = 0;
	int combos = 0;

	scanf("%s", input);
	

	//CODE HERE!
	//
	
	for(int i=0; i<MAXLEN-1; i++){
		if(input[i] == '(' && input[i+1] == '('){
			numFrontLegs[frontCtr] = i;
			frontCtr++;
		}
	}


	for(int i=0; i<MAXLEN-1; i++){
		if(input[i] == ')' && input[i+1] == ')'){
			numBackLegs[backCtr] = i;
			backCtr++;
		}
	}
	

	for(int i=0; i<frontCtr; i++){
		for(int j=0; j<backCtr; j++){
			if(numFrontLegs[i] < numBackLegs[j]){
				combos++;			
			}
		}
	}

	printf("%d \n", combos);

}
			
