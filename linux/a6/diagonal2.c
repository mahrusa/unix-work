#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 16

//use one command line argument
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: diagonal <textstring>\n");
		return -1;
	}
	
	//create a file so that 16 rows of empty will appear with od -c command
	int fd = open("diagonal2.out", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	char space = ' ';
	for(int line=0; line<LINESIZE*(argc-1); line++)
		for(int column=0; column<LINESIZE; column++)
			write(fd, &space, 1);

	int startLine = 0;
	// does this for each word
	for(int j=1; j< argc; j++){
		//Each line of od outputs 16 characters 
		//So, to make the output diagonal, we will use 0, 17, 34, ....
		int n = strlen(argv[j]);
		if(j % 2){
		
			for(int i=0; i<n; i++) {
				lseek(fd, (LINESIZE+1)*i + startLine, SEEK_SET);
				write(fd, &argv[j][i], 1);
			}
		} else {
			//int offset = 16;
			for(int i=0; i<n; i++) {
				lseek(fd, (LINESIZE-1)*(i+1) + startLine, SEEK_SET);
				write(fd, &argv[j][i], 1);
			}
		}
		startLine += 256;
		//printf("%d\n",startLine);
	}
	close(fd);
	puts("diagonal2.out has been created. Use od -c diagonal2.out to see the contents."); 

	return 0;
}
