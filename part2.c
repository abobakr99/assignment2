
#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <errno.h>          /* errno, ECHILD            */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#define _GNU_SOURCE
#include <stdbool.h>
#define MAX 100


struct Process{
	int pid;
	int arrivalTime;
	int CpuTime;
	int IOfrequency;
	int IOduration;
	
};

int arrayIndex; // also refers to how many elements in the Array (Processes in array).
struct Process procArray[MAX]; // global variable so i dont have to pass them into parameter.

void appendProcess(struct Process proc ){
	if(arrayIndex != MAX-1){
		procArray[arrayIndex] = proc;
		arrayIndex = arrayIndex + 1;
	}else{
		printf("Process Queue is Full.");
	}
}

bool inputFileReader(void){
	FILE *ifp;
	arrayIndex = 0;
	char *line = NULL;
	size_t len = 0;
	_ssize_t read;
	struct Process temp;
	
	ifp = fopen("Input.txt", "r");
	if(ifp == NULL) exit(EXIT_FAILURE);
	
	while((read = getline(&line, &len, ifp) != -1)){
		// extract data from each line and assign it to a temp struct to pass it for appending into the array.
		// we chose "/" as identifier between each data.
		//like for example, 1/0/22 corresponds to pid/arrivalTime/ExecutionTime
		
		
		
		
		
	}
	return false;
	// lastly we do qsort. to sort out the array for their priority.
}


void main(void){
	//File *ifp, *ofp; // input/output file pointers
	

	
	
	
	
	
	
	
	
	
	//ofp = fopen("Output", "w");
}
