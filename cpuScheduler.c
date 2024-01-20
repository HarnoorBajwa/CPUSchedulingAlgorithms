#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

char tasks[1151];
struct Task {
    char taskName[200];
    int arrivalTime;
    int burstTime;
    int remainBurstTime;
    int waitingTime;
    int turnaroundTime;
};
int numOfTasks = 0;


void fcfs(struct Task *task, int numOfTasks){
    int currenTime = 0;
    int waitingTime = 0;
    int turnArTime =0;
    int sumofallwaitingTimes=0;
    double avgwaitingTime=0;
    int waitingTimeArray[numOfTasks+1];

   FILE *outputFile = fopen("Output.txt","w");
    if(outputFile == NULL){
        perror("Error opening file");
        return;
    }
    fprintf(outputFile,"FCFS:\n"); 
    
    for(int i =0; i < numOfTasks; i++){
        
        if(currenTime < task[i].arrivalTime ){
            currenTime = task[i].arrivalTime;
        }
        // Updating the current time after execution
        currenTime+= task[i].burstTime;
        turnArTime = currenTime - task[i].arrivalTime;
        waitingTime = turnArTime - task[i].burstTime;
        sumofallwaitingTimes+=waitingTime;
        waitingTimeArray[i] = waitingTime;
        
        fprintf(outputFile,"%s    %d    %d\n",task[i].taskName, task[i].arrivalTime+waitingTime,currenTime);
        if( i == numOfTasks -1){
            for(int j =0 ; j < numOfTasks;j++){
                fprintf(outputFile,"Waiting Time %s: %d\n",task[j].taskName,waitingTimeArray[j]);
            }
        }
    }
    
    avgwaitingTime = (double) sumofallwaitingTimes / numOfTasks;
    avgwaitingTime = round(avgwaitingTime * 100.00)/100.0;
    fprintf(outputFile,"Average Waiting Time: %0.2f\n\n",avgwaitingTime);
    fclose(outputFile);

}
void RoundRobin(struct Task *task, int numOfTasks){
    
    struct Task *a = malloc(numOfTasks * sizeof(struct Task));

    // Initilaizing remaining bust time for all tasks 
    int newSize = numOfTasks;
    
    int timeQuantum = 4;
    int currentTime = 0;
    int waitingTime =0;
    FILE *outputFile = fopen("Output.txt","a");
    if(outputFile == NULL){
        perror("Error opening file");
        return;
    }
    fprintf(outputFile,"RR:\n");
    int allTasksComplete = 0;

    for (int i = 0; i < numOfTasks; i++) {
        a[i].remainBurstTime = task[i].burstTime;
        strcpy(a[i].taskName,task[i].taskName);
        a[i].arrivalTime = task[i].arrivalTime;
        a[i].burstTime = task[i].burstTime;
    }

    int i =0;

    while (allTasksComplete < newSize) {
        if (a[i].remainBurstTime > 0) {
            if (a[i].arrivalTime <= currentTime) {
                a[i].arrivalTime = currentTime;
            }
            int execTime;

            if (a[i].remainBurstTime < timeQuantum) {
                execTime = a[i].remainBurstTime;
                allTasksComplete++;
            } else {
                execTime = timeQuantum;
            }
            currentTime += execTime;
            a[i].remainBurstTime -= execTime;
            // Update waiting time for other tasks
            for (int j = 0; j < numOfTasks; j++) {
                if (i != j && a[j].remainBurstTime > 0) {
                    a[j].waitingTime += execTime;
                }
            }
            fprintf(outputFile,"%s    %d    %d\n", a[i].taskName, a[i].arrivalTime, currentTime);  
        }
        if(a[i].remainBurstTime !=0){

            int insertionPoint = -1;
            for (int j = 0; j < numOfTasks; j++) {
                if (a[i].arrivalTime < a[j].arrivalTime && currentTime < a[j].arrivalTime) {
                    insertionPoint = j;
                    break;
                }
                insertionPoint = newSize;
            }
            if (insertionPoint != -1 ) {
                newSize += 1;
                a = realloc(a, newSize * sizeof(struct Task));
                for (int k = newSize - 1; k > insertionPoint; k--) {
                    a[k] = a[k - 1];
                }
                a[insertionPoint]=a[i];
            }
        }
        if(a[i].remainBurstTime ==0){
            allTasksComplete++;
        }
        i++;
    }
    
    free(a); // Free allocated memory
    fclose(outputFile);
}

void sortTasksByBurstTime(struct Task *task, int numOfTasks) {
  for (int i = 0; i < numOfTasks - 1; i++) {
    for (int j = i + 1; j < numOfTasks; j++) {
      // Check if task[i] has arrival time 0; if yes, skip sorting
      if (task[i].arrivalTime == 0) {
        break;
      }
      // Sort based on burst time
      if (task[i].burstTime > task[j].burstTime || 
            (task[i].burstTime == task[j].burstTime && task[i].arrivalTime > task[j].arrivalTime)) {
        struct Task temp = task[i];
        task[i] = task[j];
        task[j] = temp;
      }
    }
  }
}

void nsjf(struct Task *task, int numOfTasks){

    struct Task *a = malloc(numOfTasks * sizeof(struct Task));
    for (int i = 0; i < numOfTasks; i++) {
        a[i].remainBurstTime = task[i].burstTime;
        strcpy(a[i].taskName,task[i].taskName);
        a[i].arrivalTime = task[i].arrivalTime;
        a[i].burstTime = task[i].burstTime;
    }
    
    sortTasksByBurstTime(task,numOfTasks);
  
    int currenTime = 0;
    int waitingTime = 0;
    int turnArTime =0;
    int sumofallwaitingTimes=0;
    double avgwaitingTime=0;
    int waitingTimeArray[numOfTasks+1];

   FILE *outputFile = fopen("Output.txt","a");
    if(outputFile == NULL){
        perror("Error opening file");
        return;
    }
    fprintf(outputFile,"\nNSJF:\n");
 
        for(int i = 0; i < numOfTasks;i++){
            if (task[i].arrivalTime <= currenTime) {
    
                currenTime += task[i].burstTime;
                turnArTime = currenTime - task[i].arrivalTime;
                waitingTime = turnArTime - task[i].burstTime;
                sumofallwaitingTimes+=waitingTime;
                waitingTimeArray[i] = waitingTime;
                fprintf(outputFile,"%s    %d    %d\n",task[i].taskName, task[i].arrivalTime+waitingTime,currenTime); 

            }
            // Update the waiting times in the original sequence
            for (int i = 0; i < numOfTasks; i++) {
                for (int j = 0; j < numOfTasks; j++) {
                    if (strcmp(a[j].taskName, task[i].taskName) == 0) {
                        a[j].waitingTime = waitingTimeArray[i];
                        break;
                    }
                }
            }
            if(i == numOfTasks -1  ){
                for(int j =0 ; j < numOfTasks;j++){
                    fprintf(outputFile,"Waiting Time %s: %d\n",a[j].taskName,a[j].waitingTime);
                }
            }
        }
        avgwaitingTime = (double) sumofallwaitingTimes / numOfTasks;
        avgwaitingTime = round(avgwaitingTime * 100.00)/100.0;
        fprintf(outputFile,"Average Waiting Time: %0.2f\n\n",avgwaitingTime);
        fclose(outputFile);
}

void psjf(struct Task *task, int numOfTasks) {

    struct Task *a = malloc(numOfTasks * sizeof(struct Task));

    int newSize = numOfTasks;

    int currentTime = 0;

    int allTasksComplete = 0;

    FILE *outputFile = fopen("Output.txt", "a");

    if (outputFile == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(outputFile, "PSJF:\n");

    for (int i = 0; i < numOfTasks; i++) {
        a[i].remainBurstTime = task[i].burstTime;
        strcpy(a[i].taskName, task[i].taskName);
        a[i].arrivalTime = task[i].arrivalTime;
        a[i].burstTime = task[i].burstTime;
        a[i].waitingTime = 0;
    }

    while (allTasksComplete < newSize) {
        int shortestJobIndex = -1;
        int shortestJobBurst = 500000;

        // Find the process with the shortest remaining burst time
        for (int j = 0; j < newSize; j++) {
            if (a[j].remainBurstTime > 0 && a[j].arrivalTime <= currentTime) {
                if (a[j].remainBurstTime < shortestJobBurst) {
                    shortestJobBurst = a[j].remainBurstTime;
                    shortestJobIndex = j;
                }
                else if (a[j].remainBurstTime == shortestJobBurst && a[j].arrivalTime < a[shortestJobIndex].arrivalTime) {
                    shortestJobIndex = j;
                }
            }
        }

        if (shortestJobIndex != -1) {
            int execTime = 1; // Preemptive execution
            currentTime += execTime;
            a[shortestJobIndex].remainBurstTime -= execTime;

            for (int j = 0; j < newSize; j++) {
                if (j != shortestJobIndex && a[j].remainBurstTime > 0) {
                    a[j].waitingTime += execTime;
                }
            }

            if (a[shortestJobIndex].remainBurstTime == 0) {
                allTasksComplete++;
            }

            fprintf(outputFile, "%s    %d    %d\n", a[shortestJobIndex].taskName, currentTime - execTime, currentTime);
        }
        else {
            currentTime++; // If no process can be scheduled, move to the next time unit
        }
    }
    free(a); // Free allocated memory
    fclose(outputFile);
}



int main(int argc , char** argv){
    /* Opening a pointer to the File to get information from */
    FILE *fileP = fopen("TaskSpec.txt","r");
    if(fileP == NULL){
        perror("Error opening file");
        return(-1);

    }
    
    while(fgets(tasks,sizeof(tasks),fileP) != NULL){
        numOfTasks++;

    }   
        // Allocate memory for tasks 
        struct Task *task = (struct Task *)malloc(numOfTasks * sizeof(struct Task));

        // Return to the beginning of the file
        fseek(fileP, 0, SEEK_SET);
        //Use sscanf to parse the line
        // Read each line of the file and store task information
        for (int i = 0; i < numOfTasks; i++) {  
            if (fgets(tasks,sizeof(tasks),fileP) != NULL) {
                // Print or process the extracted values
                sscanf(tasks, "%199[^,],%d,%d", task[i].taskName, &task[i].arrivalTime, &task[i].burstTime);
            } else {
                // Handle the case where the line is not in the expected format
                fprintf(stderr, "Error parsing line: %s", tasks);
            }
        }
    fclose(fileP);
    // Call first come first serve scheduling algorithm
    fcfs(task,numOfTasks);
    RoundRobin(task,numOfTasks);
    nsjf(task,numOfTasks);
    psjf(task,numOfTasks);

    // Free allocated memory
    free(task);
    
    return 0;

}
