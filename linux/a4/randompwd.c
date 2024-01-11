#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define MAXLEN 100


char pswd[MAXLEN]; // password
// i made an array since special chars are not contiguous
char specials[11] ={'!','@','#','$','%','^','&','*','+',';',':'};

char randLower(){
	return rand()%26 + 'a';
}

char randUpper(){
	return rand()%26 + 'A';
}

char randDigit(){
	return rand()%10 + '0';
}

char randSpecial(){
	int randPos = rand()%11;
	return specials[randPos];
}


int main (int argc, char* argv[]){

	if(argc != 7){
		puts("needs 7 args (randompwd + 6 nums)");
		return 1;
	}


	int minLen = atoi(argv[1]);
	int maxLen = atoi(argv[2]);
	int minLower = atoi(argv[3]);
	int minUpper = atoi(argv[4]);
	int minDigit = atoi(argv[5]);
	int minSpecial = atoi(argv[6]);

	srand(time(NULL) + getpid());

	// random length of password
	int len = (rand()%(maxLen-minLen)) + minLen;

	// array to see if char at index is chosen
	int *charSpace = (int*) malloc(len*sizeof(int));
	for(int i=0; i<len; i++){
		charSpace[i] = 0;
	}


	for(int i=0; i<minLower; i++){
		int choice = rand()%len;
		if(charSpace[choice]==0){
			charSpace[choice] = 1;
			pswd[choice] = randLower();
		} else {
			i--;
		}

	}

	for(int i=0; i<minUpper; i++){
		int choice = rand()%len;
		if(charSpace[choice]==0){
			charSpace[choice] = 1;
			pswd[choice] = randUpper();
		} else {
			i--;
		}

	}


	for(int i=0; i<minDigit; i++){
		int choice = rand()%len;
		if(charSpace[choice]==0){
			charSpace[choice] = 1;
			pswd[choice] = randDigit();
		} else {
			i--;
		}

	}

	for(int i=0; i<minSpecial; i++){
		int choice = rand()%len;
		if(charSpace[choice]==0){
			charSpace[choice] = 1;
			pswd[choice] = randSpecial();
		} else {
			i--;
		}

	}


	for(int i=0; i<len; i++){
		
		if(charSpace[i]==0){
		// randomly choose between upper, lower, special, digit
		int choice = rand()%4;
		int done = 0;

		switch(choice){
		
			case 0:
				pswd[i] = randLower();
				done++;
				break;
			case 1:
				pswd[i] = randUpper();
				done++;
				break;
			case 2:
				pswd[i] = randDigit();
				done++;
				break;
			case 3:
				pswd[i] = randSpecial();
				done++;
				break;

		}
		}

	}
	
	puts(pswd);

	return 0;



}
