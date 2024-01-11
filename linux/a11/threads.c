#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int numThreads;

void *work( void *ptr )
{
	int n = (int) ptr;
	printf("I am thread number %d\n", n);
     pthread_t thread1, thread2;

	if(2*n + 1 < numThreads){
     		pthread_create(&thread1, NULL, work, (void *) (2*n+1));
		if(2*n + 2 < numThreads){		
	    		 pthread_create(&thread2, NULL, work, (void *) (2*n+2));
	     		pthread_join(thread2, NULL);
		}
	     pthread_join(thread1, NULL);
    } 
}

int main(int argc, char *argv[])
{
     pthread_t thread1;

	// number of threads
	numThreads = atoi(argv[1]); 

     pthread_create(&thread1, NULL, work, (void *) 0);

    pthread_join(thread1, NULL); 
//	return 0;
}
