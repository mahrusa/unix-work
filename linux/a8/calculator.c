#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXLEN 1000

char operators[100];
int fds[201][2];
int operatorCount=0;
int numPipes=0;

void child(int index) {
	//CODE HERE!
	//reconfigure appropriate pipes (plumbing) to fds 0, 3 and 1
	//close all pipes! IF missed, you will end up with lot of orphans! 
	// operators[index] +         -        *            /
	// execl            add       subtract mult         divide
	//printf() is bad idea! --> fprintf(stderr, "...\n", 	
	
	// piping
	dup2(fds[2*index][0], 0);
	dup2(fds[2*index + 1][0], 3);
	dup2(fds[2*index + 2][1], 1);
	
	//fprintf(stderr , "child %d: piping %d %d in %d out\n", index, fds[2*index][0], fds[2*index+1][0], fds[2*index+2][1]);	
	
	for(int i=0; i<numPipes; i++){
//		if(i != 2*index && i != 2*index+1){
			//fprintf(stderr, "child %d closing %d\n", index, fds[i][0]);
			close(fds[i][0]);
//		}
//		if(i != 2*index + 2){
			//fprintf(stderr, "child %d closing %d\n", index, fds[i][1]);
			close(fds[i][1]);
//		}
	}
	

	if(operators[index] == '+'){
		execl("./add", "add",  (char *)NULL);
	} else if(operators[index] == '-'){
		execl("./subtract", "subtract",  (char *)NULL);
	} else if (operators[index] == '*'){
		execl("./multiply" , "multiply",  (char *)NULL);
	} else {
		execl("./divide" , "divide",  (char *)NULL);
	}

	fprintf(stderr, "execl failed\n");
}

int main(int argc, char *argv[]) {
	char line[MAXLEN], *temp;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	dup(0); // make 3 busy

	FILE *dataFile = fopen(argv[1], "r");
	//read the first line - it contains the configuration
	fgets(line, MAXLEN, dataFile); 

	// sample content for line: a + b - c
	strtok(line, " \n"); //skip the symbol representing variable/parameter
	int i=0;
	while (temp = strtok(NULL, " \n")) {
		operators[operatorCount] = temp[0];
		operatorCount++;
		i++;
		strtok(NULL, " \n"); //skip the symbol representing variable/parameter
	}


	//create the necessary # of pipes
	numPipes = operatorCount * 2 + 1;

	//CODE HERE!
	//loop: create that many pipes (numPipes) -- all pipes are created!
	for(int i=0; i<numPipes; i++){
		pipe(fds[i]);
//		printf("read: %d, write: %d\n", fds[i][0], fds[i][1]);
	}
	//loop: create that many children (operatorCount)
	for(int i=0; i<operatorCount; i++){
		if(!fork()){
			child(i);
			break;
		}
	}


	// close all leftover pipes
	for(int i=0; i<numPipes-1; i++)
		close(fds[i][0]);
	for(int i=2; i<numPipes; i+=2)
		close(fds[i][1]);

	
	int params = operatorCount + 1;
	int num;
	int result;
	// gets line, parses and pipes params for each line
	while(fgets(line, MAXLEN, dataFile) != 0){
		temp = strtok(line, " ");
		for(int j=0; j<params; j++){
			num = atoi(temp);
			if(j==0){
				write(fds[j][1], &num, sizeof(int));
			} else {
				write(fds[2*j -1][1], &num, sizeof(int));		 		
			}
			temp = strtok(NULL, " ");
		}
		read(fds[numPipes-1][0], &result, sizeof(int));
		printf("%d\n", result);

	}

	return 0;
}

