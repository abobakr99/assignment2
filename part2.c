#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <errno.h>          /* errno, ECHILD            */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#define _GNU_SOURCE
#include <stdbool.h>
#define MAX 100




//-----------------------------------------------------------------------//


struct Process{
	int pid;
	int arrivalTime;
	int CpuTime;
	int IOfrequency;
	int IOduration;
	char *oldState;
	char *newState;
	int waitingTime;
	int turnaroundTime;
	
};



//-----------------------------------------------------------------------//
						// Global Variables.
//-----------------------------------------------------------------------//
int arrayLength; // also refers to how many elements in the Array (Processes in array).
struct Process procArray[MAX]; // global variable so i dont have to pass them into parameter.
FILE *ofp;
int index;
char waiting[] = "Waiting State.";
char ready[] = "Ready State.";
char running[] = "Running State.";
char new[] = "New State.";
char terminated[] = "Terminated State.";
//-----------------------------------------------------------------------//
//					      	Subroutines
//----------------------------------------------------------------------//

void appendProcess(struct Process proc ){
	if(arrayLength != MAX-1){
		procArray[arrayLength] = proc;
		arrayLength = arrayLength + 1;
	}else{
		printf("Process Queue is Full.");
	}
}


void removefromArray(){


	for(int i = index; i<arrayLength; i++) { procArray[i] = procArray[i+1]; }

	arrayLength--;
}

//void sleep(unsigned milliseconds){
	//usleep(milliseconds*1000);
//}

void inputFileReader(void){
	FILE *ifp;
	arrayLength = 0;
	index = 0;
	struct Process temp;
	ifp = fopen("Input.txt", "r");
	if(ifp == NULL) exit(1);
	int pid, arr, exe, iod, iof;
	while(fscanf(ifp, "%d %d %d %d %d", &pid, &arr, &exe, &iod, &iof) != EOF){
		
		// we assign the pid and arrival time and execution time for a process then append it in array of processes.
		temp.pid = pid; 
		temp.arrivalTime = arr; 
		temp.CpuTime = exe;
		temp.IOduration = iod;
		temp.IOfrequency = iof;
		temp.oldState = new;
		temp.newState = ready;
		temp.turnaroundTime = temp.waitingTime = 0;
		//---------sending temp to appendProcess---------//
		appendProcess(temp);
		
	}
	fclose(ifp);
	
	// lastly we do qsort. to sort out the array for their priority.
}

void out(char str[]){
	

	fputs(str, ofp);
	
}


void printProcess(struct Process *p, int time){
        printf("| %2d  |  %2d  |    %s    |    %s    |\n"
               , time, p->pid, p->oldState, p->newState );
}




struct Process * scheduler(int t){ // picks who is next,
	// based on our policy, its FCFS.
	
	for(;1;index++){
		if(index == arrayLength){
			index = 0;
			struct Process *p = NULL;
			return p;
		}
		if(procArray[index].arrivalTime <= t && procArray[index].CpuTime > 0){
			procArray[index].oldState = procArray[index].newState;
			procArray[index].newState = running;
			return &procArray[index];
		}
		
	}

}






void simulator(){
	struct Process * currentProcess;
	int timeUnit = 0;
	while(arrayLength != 0){
		while((currentProcess = scheduler(timeUnit))!= NULL ){
			printProcess(currentProcess, timeUnit);

			



			if(currentProcess->CpuTime == 0){
				removefromArray();
			}
		}


	}
}











int main(void){

	// --------------- asking to read input file -------//
		inputFileReader(); // after this all process are stored in global access procArray.
		//checking the array.
		printf("How many process read: %d\n", arrayLength);
		procArray[0].turnaroundTime = procArray[0].CpuTime;
		for(int i=1; i<arrayLength; i++){
			procArray[i].waitingTime = procArray[i-1].waitingTime + procArray[i-1].CpuTime;
			procArray[i].turnaroundTime = procArray[i].waitingTime + procArray[i].CpuTime;
	    }
		
	//---------------- prepare output file to be used frequently as "Log"------///
	ofp = fopen("Output.txt", "w");
	if(ofp == NULL){
		printf("ERROR\n");
		exit(1);
	}
	//--------- the start of our proccess scheduling -----------//
	// FCFS scenario.
	// all process should be ready to run.
    	puts("+-------+-----+------------------+--------------------+");
   		puts("| Time  | PID |    Old State     |     New State      |");
    	puts("+-------+-----+------------------+--------------------+");
		procSimulator();




	fclose(ofp);
	return 1;
}




