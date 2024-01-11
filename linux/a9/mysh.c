#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

//read all the quotes from quotes.txt
//when client asks for a motivational quote, select one randomly and send it out.

#define MAXQUOTES 10000
#define MAXLEN 1000


char *quotes[MAXQUOTES];
int numQuotes=0;
char *commands[MAXLEN];
int fds[MAXLEN][2];
int numCommands;

//runs a simple command
//cmdname arg1 arg2 arg3 ...
void runCommand(char *command) {
	//split and assemble the arguments and invoke execvp()
	//use strtok(..)
	// RUNCMD2.C
	char *args[MAXLEN];
	int index = 0;
	//tokenize command string: strtok()
	args[index] = strtok(command, " \n");
	while (args[index]) 
		args[++index] = strtok(NULL, " \n");

	//invoke execvp
	execvp(args[0], args);
	fprintf(stderr, "This is bad!\n");
	exit(1);
}

//cmd0 | cmd1 | cmd2 | cmd3 | cmd4 ....
void child(int i) {
	//rewire pipes to 0 and 1 
	//do NOT rewire 0 for the first command - leave it as std input
	//do NOT rewrite 1 for the last command - leave it as std output
	if(i>0){
		dup2(fds[i-1][0], 0);
	}
	if(i<numCommands-1){
		dup2(fds[i][1], 1);
	}
	
	//close unnecessary pipes - for loop?
	for(int j=0; j<numCommands-1; j++){
		close(fds[j][0]);
		close(fds[j][1]);
	}
	//run ith command
	runCommand(commands[i]);
}

void processLine(char *line) {
	char *pipePtr = strchr(line, '|'); //does this command have | chars?
	char *equalPtr = strchr(line, '='); //does this command have =?
	if (pipePtr) { //not NULL - cmd1 | cmd2 | cmd3 ....
		//command has several sub-commands connected with pipes
		//setup commands array
		//setup pipes array
		//fork & create children --> invoke child(i) in a loop
		//cmd0 | cmd1 | cmd2 | cmd3 | cmd4 
		// invoke child(i) for the last command directly?
		
		//setup commands array
		char *temp = strtok(line, "|");
		int i=0;
		while (temp != NULL){		
			commands[i] = (char *) malloc (strlen(temp)+1);
			strcpy(commands[i], temp);
			i++;
			temp = strtok(NULL, "|");
		}
		numCommands = i;
		//setup pipes array
		for(i=0; i<numCommands-1; i++){
			pipe(fds[i]);
		}
		
		//fork & create children --> invoke child(i) in a loop
		for(i=0; i<numCommands; i++){
			if(!fork())
				child(i);
		}

	} else if (equalPtr) { 
		//command has = operator, so 2 commands --> 2 processes
		// CONNECT.C
		char *cmd1 = strtok(line, "=");
		char *cmd2 = strtok(NULL, "\n");
		int tochild[2], toparent[2];
		pipe(tochild);
		pipe(toparent);
		if(fork()){
			dup2(toparent[0], 0);
			dup2(tochild[1], 1);
			close(tochild[0]); close(tochild[1]);
			close(toparent[0]); close(toparent[1]);
			runCommand(cmd1);
		} else {
			dup2(tochild[0], 0);
			dup2(toparent[1], 1);
			close(tochild[0]); close(tochild[1]);
			close(toparent[0]); close(toparent[1]);
			runCommand(cmd2);
		}

    	} else {
		//it is a simple command, no pipe or = character
		runCommand(line);

	}
}

int main() {
	// load up all the quotes from quotes.txt
	// quoteserver.c
		
	FILE *fp = fopen("quotes.txt", "r");
	int i=0;
	char line[MAXLEN];
	while (fgets(line, MAXLEN, fp)) {
		quotes[i] = (char *) malloc (strlen(line)+1);
		strcpy(quotes[i], line);
		i++;
		//printf("error here at i = %d\n", i);
	}
	numQuotes = i;
	
	//printf("error here at i = %d\n", i);
	// infinite loop to serve the customer
	srand(time(NULL));
	while (1) {
		//output a random quote to stderr
		fputs(quotes[rand()%numQuotes], stderr);
		fprintf(stderr, "# ");
		//get the user input
		fgets(line, 1000, stdin);

		//spawn a child for taking care of it
		if (fork() == 0) 
			processLine(line);

		//wait the child to finish the job!
		wait(NULL);
	}
}
