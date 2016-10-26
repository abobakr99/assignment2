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
FILE *ofp;


void appendProcess(struct Process proc ){
	if(arrayIndex != MAX-1){
		procArray[arrayIndex] = proc;
		arrayIndex = arrayIndex + 1;
	}else{
		printf("Process Queue is Full.");
	}
}

void inputFileReader(void){
	FILE *ifp;
	arrayIndex = 0;
	struct Process temp;
	ifp = fopen("Input.txt", "r");
	if(ifp == NULL) exit(1);
	int pid, arr, exe;
	while(fscanf(ifp, "%d %d %d", &pid, &arr, &exe) != EOF){
		
		// we assign the pid and arrival time and execution time for a process then append it in array of processes.
		temp.pid = pid; 
		temp.arrivalTime = arr; 
		temp.CpuTime = exe;
		temp.IOduration = 1;
		temp.IOfrequency = 1;
		//---------sending temp to appendProcess---------//
		appendProcess(temp);
		
	}
	fclose(ifp);
	
	// lastly we do qsort. to sort out the array for their priority.
}

void out(char str[]){
	

	fputs(str, ofp);
	
}

int main(void){

	// --------------- asking to read input file -------//
		inputFileReader(); // after this all process are stored in global access procArray.
		//checking the array.
		printf("How many process read: %d\n", arrayIndex);
		for(int i=0; i<arrayIndex; i++){
			printf(" %d," , procArray[i].pid);
	    }
		printf("\n");

	//---------------- prepare output file to be used frequently as "Log"------///
		ofp = fopen("Output.txt", "w");
	if(ofp == NULL){
		printf("ERROR\n");
		exit(1);
	}
	
	fclose(ofp);
	return 1;
}








