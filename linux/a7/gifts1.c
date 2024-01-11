#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//let us assume that there will not be more than 100 players
#define MAXPLAYERS 100
//let us assume that max length for any name is 100 characters
#define MAXLEN 100


//arrays to store the player names and balances
char names[MAXPLAYERS][MAXLEN];
double balances[MAXPLAYERS];
int numPlayers = 0; //set when actual player data is loaded

//search the player names array and return the index if specified name is found
//return -1 otherwise.
int findIndex(char *name) {
        for(int i=0; i<numPlayers; i++)
            if(strcmp(name, names[i]) == 0)
               return i;

        return -1;
}

int ctLines(char *filename){
	int ct = 0;
	char c;
	FILE *lineCtr = fopen(filename, "r");
	while(!feof(lineCtr)){
		c = fgetc(lineCtr);
		if(c == '\n')
			ct++;
	}
	fclose(lineCtr);
	return ct;
}

int main(int argc, char *argv[]) {
	//code here!
	
	if(strcmp(argv[1], "new") == 0){
		// initiazes player values
		for(int i=0; i<(argc-2)/2; i++){
			strcpy(names[i], argv[2+2*i]);
			balances[i] = atof(argv[3+2*i]);
			numPlayers++;
		}
	} else {
		numPlayers = ctLines("gifts1.txt");
		FILE *filep = fopen("gifts1.txt", "r"); 
		// read cuurent balances from file
		for(int i=0; i<numPlayers; i++){
			
			fscanf(filep, "%s %lf\n", names[i], &balances[i]);
		}
		int giver = findIndex(argv[1]);
		double amt = atof(argv[2]);
		balances[giver] -= amt;
		for(int i=3; i<argc; i++){
			int recipient = findIndex(argv[i]);
			balances[recipient] += amt/(argc-3);
		}
		fclose(filep);

	}

	FILE *filep = fopen("gifts1.txt", "w");

	for(int i=0; i<numPlayers; i++){
		fprintf(filep, "%s %lf\n", names[i], balances[i]);
	}
	fclose(filep);
	// output current balances
	for(int i=0; i<numPlayers; i++){
		printf("%10s: %6.2lf\n", names[i], balances[i]);
	}
}

