#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>
#include <signal.h>

void pexit(char *errmsg) {
	fprintf(stderr, "%s\n", errmsg);
	exit(1);
}


int clientfds[1024]; //stores the connection fds for each client
char clientNames[1024][200]; //stores the connection fds for each client
int nextClientIndex = 0;

//function for dedicated server thread
void *child(void *ptr) {
	uint32_t connfd = (uint32_t) ptr;
	int index = nextClientIndex++;
	clientfds[index] = connfd;
	char *prompt = "Enter your name: \n";
	write(connfd, prompt, strlen(prompt));
	read(connfd, clientNames[index], 200);
	char *cptr = strchr(clientNames[index], '\n');
	if (cptr)
		*cptr = '\0';
	char buffer[1024];
	sprintf(buffer, "Welcome, %s! \n\n", clientNames[index]);
	write(connfd, buffer, strlen(buffer));

	for(int i=0; i<nextClientIndex; i++)
		if(i != index){
			sprintf(buffer, "%s joined! \n\n", clientNames[index]);
			write(clientfds[i], buffer, strlen(buffer));
		}
	//wait for answer from client
	while (read(connfd, buffer, 1024)) {
		char *token = strtok(buffer, " \n");
		if (!strcmp(token, "list")){ //mismatch?
			for(int i=0; i<nextClientIndex; i++){
				sprintf(buffer, "%s\n", clientNames[i]);
				write(connfd, buffer, strlen(buffer));
			}
		}else if (!strcmp(token, "send")){
			token = strtok(NULL, " ");
			int rIndex;
			for(rIndex=0; rIndex<nextClientIndex; rIndex++){
				if(!strcmp(clientNames[rIndex], token))
					break;
			}
			token = strtok(NULL, "\n");
			sprintf(buffer, "%s says %s\n", clientNames[index], token);
			write(clientfds[rIndex], buffer, strlen(buffer));
			/*
		} else if(!strcmp(token, "check")){
			token = strtok(NULL, " \n");
			int rIndex;
			for(rIndex=0; rIndex<nextClientIndex; rIndex++){
				if(!strcmp(clientNames[rIndex], token))
					break;
			}
			if(read(clientfds[rIndex], buffer, 1024) == EOF)
				sprintf(buffer, "%s is inactive \n", token);
			else
				sprintf(buffer, "%s is active \n", token);
			*/
		}
		char *newL = "\n";
		write(connfd, newL, 2);
		//send result back to client
		//for correct answers, increment rank too.
		//write(connfd, buffer, strlen(buffer));
	}
}

int main(int argc, char *argv[])
{
	for(int sig=0; sig<=64; sig++){
		signal(sig, SIG_IGN);
	}
    
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char buffer[1025];
    time_t ticks; 

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		pexit("socket() error.");
		
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buffer, '0', sizeof(buffer)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int port = 4999;
	do {
		port++;
    	serv_addr.sin_port = htons(port); 
    } while (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0);
	printf("bind() succeeds for port #%d\n", port);

    if (listen(listenfd, 10) < 0)
		pexit("listen() error.");

	//one more thread to send the questions?
    pthread_t thread1;
    //pthread_create(&thread1, NULL, manager_child, NULL);
	
	int counter=0;
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		counter++;
		fprintf(stderr, "connected to client %d.\n", counter);
		
        pthread_create(&thread1, NULL, child, (void *) connfd);

     }
}
