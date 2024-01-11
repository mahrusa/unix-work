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

struct Player {
	char name[MAXLEN];
	double balance;
} ;

//struct is like class - we can use an array of struct (we can use like an array of objects).
struct Player players[MAXPLAYERS];
int numPlayers = 0; //set when actual player data is loaded

//search the player names array and return the index if specified name is found
//return -1 otherwise.
int findIndex(char *name) {
        for(int i=0; i<numPlayers; i++)
            if(strcmp(name, players[i].name) == 0)
               return i;

        return -1;
}

// use binary data file gifts2.dat to read and store the results.

int main(int argc, char *argv[]) {
	//code here
	//
	if(strcmp(argv[1], "new") == 0){
        	// initialize player values
                for(int i=0; i<(argc-2)/2; i++){
                	strcpy(players[i].name, argv[2+2*i]);
                	players[i].balance = atof(argv[3+2*i]);
                	numPlayers++;
               	}
	} else {
		FILE *filep = fopen("gifts3.dat", "rb");
		fread(&numPlayers, sizeof(int), 1, filep);
		
		fread(players, sizeof(struct Player), numPlayers, filep);
		int giver = findIndex(argv[1]);
                double amt = atof(argv[2]);
                players[giver].balance -= amt;
                for(int i=3; i<argc; i++){
	                int recipient = findIndex(argv[i]);
        	        players[recipient].balance += amt/(argc-3);
                }

		fclose(filep);
	}
	FILE *filep = fopen("gifts3.dat", "wb");
	fwrite(&numPlayers, sizeof(int), 1, filep);

	fwrite(players, sizeof(struct Player), numPlayers, filep);
	fclose(filep);

	 //output current balances
        for(int i=0; i<numPlayers; i++){
	        printf("%10s: %6.2lf\n", players[i].name, players[i].balance);
        }
}
