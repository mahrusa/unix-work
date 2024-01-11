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

// use binary data file gifts2.dat to read and store the results.

int main(int argc, char *argv[]) {
	//code here!
	//
	
	if(strcmp(argv[1], "new") == 0){
	 	// initialize player values
		for(int i=0; i<(argc-2)/2; i++){
              		strcpy(names[i], argv[2+2*i]);
              		balances[i] = atof(argv[3+2*i]);
	                numPlayers++;
	        }
	} else {
		// (DO) find out numPLayers somehow first
		
		int fd = open("gifts2.dat", O_RDONLY);
		read(fd, &numPlayers, sizeof(int));

		//  (TRANSLATE TO BINARY) read cuurent balances from file
		// for(int i=0; i<numPlayers; i++){
		//    fscanf(filep, "%s %lf\n", names[i], &balances[i]);
		// }
		for(int i=0; i<numPlayers; i++){
			// (TRANSLATE TO BINARY) write current balance to file
			char *j = &names[i][0];
			read(fd, j, sizeof(char));
			j++;
			while(*(j-1) != '\0'){
				read(fd, j, sizeof(char));
				j++;
			}
			// write(fd, '\0', sizeof(char));
			double *bal = &balances[i];
			read(fd, bal, sizeof(double));

		}

		int giver = findIndex(argv[1]);
		double amt = atof(argv[2]);
		balances[giver] -= amt;
		for(int i=3; i<argc; i++){
		   int recipient = findIndex(argv[i]);
		   balances[recipient] += amt/(argc-3);
		}

		close(fd);
	}

	// writing to file
	int fd = open("gifts2.dat", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	// write numPlayers first
	write(fd, &numPlayers, sizeof(int));
	
	//write(fd, balances, sizeof(double)); // will delete later

	for(int i=0; i<numPlayers; i++){
	// (TRANSLATE TO BINARY) write current balance to file
		char *j = &names[i][0];
		while(*j != '\0'){
			write(fd, j, sizeof(char));
			j++;
		}
		char null = '\0';
		write(fd, &null, sizeof(char));
		double *bal = &balances[i];
		write(fd, bal, sizeof(double));
	

	}
	
	close(fd);

	//output current balances
	for(int i=0; i<numPlayers; i++){
	         printf("%10s: %6.2lf\n", names[i], balances[i]);
	}
}
