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
	int waitingTime;
	int priority;
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
	ifp = fopen("Input.txt", "r");
	if(ifp == NULL) exit(1);
	int pid, arr, exe, pri, iof, iod;
	while(fscanf(ifp, "%d %d %d %d %d %d", &pid, &arr, &exe, &pri, &iof, &iod) != EOF){
		
		// we assign the pid and arrival time and execution time for a process then append it in array of processes.
         
		temp.pid = pid; 
		temp.arrivalTime = arr; 
		temp.priority = pri;    
		temp.CpuTime = exe;
		temp.IOduration = iod;
		temp.IOfrequency = iof;
		
		
		//---------sending temp to appendProcess---------//
		appendProcess(temp);
		
	}
	fclose(ifp);
	
	// lastly we do qsort. to sort out the array for their priority.
}
int highestPriority(){
	int maxIndex=0;
	for(int i =0; i <arrayIndex; i++){
		if(procArray[i].priority >= procArray[maxIndex].priority){
			maxIndex = i;
		}
	}
	return maxIndex;
		
}
/*
slectNextPro() chooses the first processes from the reay queue 
*/
 struct Process * selectNextPro(){
			int i = highestPriority(); // index of highest priority.
		  struct  Process p = procArray[i];

	       p.oldState =ready;
	       p.newState = running;
		   currentProcess = p;
	       arrayIndex--;
	      //---------------------------//
		   removefromArray(i);
	      // -------------------------//
	        return &currentProcess;
			
	
}

void calculateWaitingTime (){
           int i; 
           
        procArray[0].waitingTime = 0;
         printf(" waiting times %d \n",  procArray[0].waitingTime);
         
     for (i =1; i< arrayIndex ; i++) {
     
          
         procArray[i].waitingTime = procArray[i-1].waitingTime + procArray[i-1].CpuTime;
         printf(" waiting times %d  %d \n", procArray[i].pid, procArray[i].waitingTime);
        
     }
      printf(" waiting times pid 3 %d \n",  procArray[2].waitingTime);
}

void printProcess(struct Process *p, int t){
        
 
        
        printf("\n\t%d\t\t%d\t%s\t%s\t%d\n",t /*p->waitingTime*/,p->pid,p->oldState,p->newState, p->priority);
               
       // printf("\n\t%d\t\t%d\t%s\t%s\n", p->CpuTime + p->arrivalTime,p->pid,p->oldState,p->newState);
                
	

}    


void run (){
	  struct Process * p ;
     calculateWaitingTime ();
     printf("\n Transition Time       pid \t Old state\t New state\t Priority");
      
   
  ///int waitingTime=0,TransitionTime=0;
    int timeUnit = 0;
   while (( arrayIndex > 0)) {
   	 if((p = selectNextPro())!= NULL){
      
     
      		printProcess(p, timeUnit);
			int k = timeUnit;
			while(timeUnit != (k + p->IOfrequency)){
				timeUnit++;
				p->CpuTime = p->CpuTime - 1;
				
			}
			if(p->CpuTime == 0){
			   p->oldState =running;
			   p->newState =terminated ;
			   printProcess(p, timeUnit);
			}else{
			  p->oldState = p->newState;
			  p->newState = waiting;
			  printProcess(p, timeUnit);
			  p->oldState = p->newState;
			  p->newState = ready;
			  printProcess(p, timeUnit);
			  appendProcess(currentProcess);
			}
        }else{
			timeUnit++;
		}
    // sleep(1); 
     
  } 
   
    
}
int main(void){
        
      
	// --------------- asking to read input file -------//
		inputFileReader(); // after this all process are stored in global access procArray.
		printf("How many process read: %d\n", arrayIndex);
		run();
		
		return 1;
}
