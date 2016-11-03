#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <errno.h>          /* errno, ECHILD            */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#define _GNU_SOURCE
#include <stdbool.h>
#include <unistd.h>


#define MAX 100
#define RUNNING    1
#define WAITING    2
#define READY      3
#define TERMINATED 4


//-----------------------------------------------------------------------//


struct Process{
	int pid;
	int arrivalTime;
	int CpuTime;
	int IOfrequency;
	int IOduration;	
	int waitTime;
  int turnaround;
    char *oldState;
	char *newState;
};



//-----------------------------------------------------------------------//
						// Global Variables.
//-----------------------------------------------------------------------//
int arrayIndex; // also refers to how many elements in the Array (Processes in array).
struct Process procArray[MAX]; // global variable so i dont have to pass them into parameter.
FILE *ofp;
int currentProcIndex = 0;
struct Process currentProcess;

char waiting[] = "Waiting State.";
char ready[] = "Ready State.";
char running[] = "Running State.";
char new[] = "New State.";
char terminated[] = "Terminated State.";
int turnaroundtotal, waitTimetotal;
//-----------------------------------------------------------------------//
//             FUNCTION PROTOTYPES 

//appendProcess(struct Process proc );
//----------------------------------------------------------------------//
  /*
   appendProcess() Creates the ready queue 
*/
void appendProcess(struct Process proc ){
           
	if(arrayIndex != MAX-1){
		procArray[arrayIndex] = proc;
		arrayIndex = arrayIndex + 1;
	}else{
		printf("Process Queue is Full.");
	}
}
void removefromArray(int in){
	for(int i = in; i<arrayIndex; i++) {
 		procArray[i] = procArray[i+1];
	}
}

void inputFileReader(void){
       
	FILE *ifp;

	struct Process temp;
	ifp = fopen("Input_fcfs.txt", "r");
	if(ifp == NULL){
	  printf("Error opening file!\n");
	  exit(1);
	}
	
	int pid, arr, exe,i=0;
	while(fscanf(ifp, "%d %d %d", &pid, &arr, &exe) != EOF){
		
		// we assign the pid and arrival time and execution time for a process then append it in array of processes.
         
		temp.pid = pid; 
		temp.arrivalTime = arr; 
		if(i==0){
		  i++;
		  temp.waitTime = 0;
		  temp.turnaround = exe;
		}else{

		  temp.turnaround = procArray[i-1].turnaround + exe;

		  temp.waitTime = procArray[i-1].turnaround - temp.arrivalTime;

		  i++;
		  
		}
		temp.CpuTime = exe;
		temp.IOduration = 1;
		temp.IOfrequency = 1;
		
		
		//---------sending temp to appendProcess---------//
		appendProcess(temp);
		
	}
	fclose(ifp);
	
	// lastly we do qsort. to sort out the array for their priority.
}

/*
slectNextPro() chooses the first processes from the reay queue 
*/
 struct Process * selectNextPro(int t){
     for(int i = 0; i<arrayIndex; i++){
		struct  Process p = procArray[i];
		if(p.arrivalTime <= t){
	       p.oldState =ready;
	       p.newState = running;
		   currentProcess = p;
	       arrayIndex--;
	      //---------------------------//
		   removefromArray(i);
	      // -------------------------//
	        return &currentProcess;
		}
	}
	return NULL;
	
}

void printWaitingTime (){
           int i;  
     for (i =0; i< arrayIndex ; i++) {
       fprintf(ofp," pid %d waiting %d turnaround %d \n", procArray[i].pid, procArray[i].waitTime,
		procArray[i].turnaround);
        
     }
      
}

void printProcess(struct Process *p, int t){
        
 
        
  fprintf(ofp, "\n\t%d\t\t%d\t%s\t%s\n",t /*p->waitingTime*/,p->pid,p->oldState,p->newState);
               
                     
	

}    


void run (){
	  struct Process * p ;
     printWaitingTime ();
     fprintf(ofp, "\n Transition Time       pid \t Old state\t New state\n");
      
   
  ///int waitingTime=0,TransitionTime=0;
    int timeUnit = 0;
   while (( arrayIndex > 0)) {
   	 if((p = selectNextPro(timeUnit))!= NULL){
      
     
      		printProcess(p, timeUnit);
			int k = timeUnit;
			while(timeUnit != (k + p->CpuTime)){
				timeUnit++;
				
			}
            p->oldState =running;
	        p->newState =terminated ;
			printProcess(p, timeUnit);
        }else{
			timeUnit++;
		}
    // sleep(1); 
     
  } 
   
    
}
int main(void){
        
      
                // asking to read input file
		inputFileReader(); // after this all process are stored in global access procArray.
		printf("How many process read: %d\n", arrayIndex);
		// open output file
		ofp = fopen("Output_fcfs.txt", "w");
		if(ofp == NULL){
		  printf("Error opening file!\n");
		  exit(1);
		}
		// run the simulation
		run();
		// close the output file
		fclose(ofp);
		
		return 1;
}

