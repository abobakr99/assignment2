#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <errno.h>          /* errno, ECHILD            */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#define _GNU_SOURCE
#include <stdbool.h>
#define MAX 100
#include <unistd.h>



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
int arrayIndex; // also refers to how many elements in the Array (Processes in array).
struct Process procArray[MAX]; // global variable so i dont have to pass them into parameter.
FILE *ofp;
char waiting[] = "Waiting State.";
char ready[] = "Ready State.";
char running[] = "Running State.";
char new[] = "New State.";
char terminated[] = "Terminated State.";
//-----------------------------------------------------------------------//
//					      	Subroutines
//----------------------------------------------------------------------//

void appendProcess(struct Process proc ){
	if(arrayIndex != MAX-1){
		procArray[arrayIndex] = proc;
		arrayIndex = arrayIndex + 1;
	}else{
		printf("Process Queue is Full.");
	}
}


void removeFirst(){
	for(int i = 0; i<arrayIndex; i++) procArray[i] = procArray[i+1];

	arrayIndex--;
}

//void sleep(unsigned milliseconds){
	//usleep(milliseconds*1000);
//}

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


void print_table(struct Process p[]){
    int i;

    puts("+-----+---------------------+------------------+--------------------+");
    puts("| PID |  Time of transition |    Old State     |     New State      |");
    puts("+-----+---------------------+------------------+--------------------+");

    for(i=0; i<arrayIndex; i++) {
        printf("| %2d  |         %2d          |    %s    |    %s    |\n"
               , p[i].pid, p[i].waitingTime , p[i].oldState, p[i].newState );
        puts("+-----+---------------------+------------------+--------------------+");
    }
	printf("\n");

}

void procSimulator(){
	
	// int i here refers on the time in millisconds.
	for(int i = 0;i<arrayIndex; i++){ // means theres no more process in ready state , break.

		procArray[i].oldState = procArray[i].newState;
		procArray[i].newState = running;
		
		print_table(procArray);
		fflush(stdout);
		Sleep(procArray[i].CpuTime*100); // to be able to see the transitions.

		procArray[i].oldState = procArray[i].newState;
		procArray[i].newState = terminated;

		print_table(procArray);

	}



}


int main(void){

	// --------------- asking to read input file -------//
		inputFileReader(); // after this all process are stored in global access procArray.
		//checking the array.
		printf("How many process read: %d\n", arrayIndex);
		procArray[0].turnaroundTime = procArray[0].CpuTime;
		for(int i=1; i<arrayIndex; i++){
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
		print_table(procArray);
		procSimulator();




	fclose(ofp);
	return 1;
}




