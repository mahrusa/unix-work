// Mahrusa Billah mxb210062

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define SIBLOCKS 2048L
#define DIBLOCKS (2048L*2048L)
#define TIBLOCKS (2048L*2048L*2048L)

long computeOverheadBlocks(long);

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Usage: diskblocks <file size in KBs>\n");
		return 1;
	}
	long filesize = atol(argv[1]);
	long diskblocks = filesize / 8;
	if (filesize % 8) { 
		diskblocks++;
	}
	printf("%ld %ld\n", diskblocks, computeOverheadBlocks(diskblocks) );
	return 0;

}


long computeOverheadBlocks(long diskblocks) {
	//CODE HERE!
	long blocksLeft = diskblocks;  
	long runningSum = 0;
	
	long numSIPs;
	long numDIPs;

	// if all 12 dps used, go to SIP
	if(blocksLeft > 12){
		blocksLeft -= 12;
		runningSum++;
		
		// if SIP full, go to DIP
		if(blocksLeft > SIBLOCKS){
			runningSum++;
			blocksLeft -= SIBLOCKS;

			// if DIP full, go to TIP
			if(blocksLeft > DIBLOCKS){
				runningSum += SIBLOCKS;

				blocksLeft -= DIBLOCKS;

				// if TIP full, doesnt work
				if(blocksLeft > TIBLOCKS){ 
					runningSum = -1;
				} 
				// if TIP not full, calculate num of child DIPs and SIPs used
				else {
					numSIPs =  blocksLeft / SIBLOCKS;
					numDIPs = blocksLeft / DIBLOCKS;
					if(blocksLeft % SIBLOCKS){
						numSIPs++;
					}
					if(blocksLeft % DIBLOCKS){
						numDIPs++;
					}

					runningSum += numSIPs + numDIPs + 1L;
				}
			}
			// if DIP not full, calculate num of child SIPS used
			else {
				numSIPs = blocksLeft / SIBLOCKS;
				if(blocksLeft % SIBLOCKS){
					numSIPs++;
				}

				runningSum += numSIPs;
			}

		} 
	
	
	}

	return runningSum;
}
