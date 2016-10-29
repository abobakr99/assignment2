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
        char *oldState;
	char *newState;
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


void inputFileReader(void){
       
	FILE *ifp;

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

/*
slectNextPro() chooses the first processes from the reay queue 
*/
 struct Process selectNextPro(){
      
       struct  Process p = procArray[0]; 
               p.oldState =ready;
	            p.newState = running;
                arrayIndex--;
       //printf(" sate  : %s\n", p.newState );
	for(int i = 0; i<arrayIndex; i++) {
          procArray[i] = procArray[i+1];
	}
       
        return p;
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

void printProcess(struct Process p){
        
        int TransitionTime , waitingTime;
        
        printf("\n\t%d\t\t%d\t%s\t%s\n",p.waitingTime,p.pid,p.oldState,p.newState);
                
                p.oldState =running;
	        p.newState =terminated ;
        printf("\n\t%d\t\t%d\t%s\t%s\n", p.CpuTime + p.waitingTime,p.pid,p.oldState,p.newState);
                
	

}    


void run (){
	  struct Process p ;
     calculateWaitingTime ();
     printf("\n Transition Time       pid \t Old state\t New state");
      
   
  ///int waitingTime=0,TransitionTime=0;
    
   while (( arrayIndex > 0)) {
   	
      p = selectNextPro();
     
      printProcess(p);
        
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

