// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"

void findWaitingTimeRR(ProcessType process_list[], int process_count, int time_quantum) 
{ 
  int remaining_burst_time[process_count];
  for (int i = 0; i < process_count; i++){
    remaining_burst_time[i] = process_list[i].bt;
  }
  
  int current_time = 0;
  while (1) {
    int all_done = 1;
    for (int i = 0; i < process_count; i++){
      if (remaining_burst_time[i] > 0){
        all_done = 0;
        if (remaining_burst_time[i] > time_quantum){
          current_time += time_quantum;
          remaining_burst_time[i] -= time_quantum;
        } else {
          current_time += remaining_burst_time[i];
          process_list[i].wt = current_time - process_list[i].bt;
          remaining_burst_time[i] = 0;
        }
      }
    }
    if (all_done == 1){
      break;
    }
  }
} 

void findWaitingTimeSJF(ProcessType process_list[], int process_count)
{ 
  int remaining_burst_time[process_count];
  for (int i = 0; i < process_count; i++){
    remaining_burst_time[i] = process_list[i].bt;
  }

  int completed_processes = 0;
  int current_time = 0;
      
  while (completed_processes < process_count){
    int min_burst_time = INT_MAX;
    int selected_process = -1;
    for (int i = 0; i < process_count; i++){
      if (remaining_burst_time[i] > 0 && remaining_burst_time[i] < min_burst_time && process_list[i].art <= current_time){
        min_burst_time = remaining_burst_time[i];
        selected_process = i;
      }
    }
    remaining_burst_time[selected_process]--;
    current_time++;

    if (remaining_burst_time[selected_process] == 0){
      completed_processes++;
      process_list[selected_process].wt = current_time - process_list[selected_process].art - process_list[selected_process].bt;
    }
  }
} 

void findWaitingTime(ProcessType process_list[], int process_count)
{ 
    process_list[0].wt = 0 + process_list[0].art; 
    for (int i = 1; i < process_count; i++) 
        process_list[i].wt = process_list[i-1].bt + process_list[i-1].wt; 
} 
  
void findTurnAroundTime(ProcessType process_list[], int process_count)
{ 
    for (int i = 0; i < process_count; i++) 
        process_list[i].tat = process_list[i].bt + process_list[i].wt; 
} 

int my_comparer(const void *first, const void *second)
{ 
    ProcessType *proc1 = (ProcessType *)first;
    ProcessType *proc2 = (ProcessType *)second;

    if (proc1->pri > proc2->pri){
      return -1;
    } else if (proc1->pri < proc2->pri){
      return 1;
    } else {
      return 0;
    }
} 

void findavgTimeFCFS(ProcessType process_list[], int process_count) 
{ 
    findWaitingTime(process_list, process_count); 
    findTurnAroundTime(process_list, process_count); 
    printf("\n*********\nFCFS\n");
}

void findavgTimeSJF(ProcessType process_list[], int process_count) 
{ 
    findWaitingTimeSJF(process_list, process_count); 
    findTurnAroundTime(process_list, process_count); 
    printf("\n*********\nSJF\n");
}

void findavgTimeRR(ProcessType process_list[], int process_count, int time_quantum) 
{ 
    findWaitingTimeRR(process_list, process_count, time_quantum); 
    findTurnAroundTime(process_list, process_count); 
    printf("\n*********\nRR Quantum = %d\n", time_quantum);
}

void findavgTimePriority(ProcessType process_list[], int process_count) 
{ 
    qsort(process_list, process_count, sizeof(ProcessType), my_comparer);
    findWaitingTime(process_list, process_count);
    findTurnAroundTime(process_list, process_count);
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType process_list[], int process_count)
{
    int total_wait_time = 0, total_turnaround_time = 0; 
    float average_wait_time, average_turnaround_time;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
    for (int i = 0; i < process_count; i++) { 
        total_wait_time += process_list[i].wt; 
        total_turnaround_time += process_list[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", process_list[i].pid, process_list[i].bt, process_list[i].wt, process_list[i].tat); 
    } 
  
    average_wait_time = ((float)total_wait_time / (float)process_count);
    average_turnaround_time = ((float)total_turnaround_time / (float)process_count);
    
    printf("\nAverage waiting time = %.2f", average_wait_time); 
    printf("\nAverage turn around time = %.2f\n", average_turnaround_time); 
} 

ProcessType * initProc(char *filename, int *process_count) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *process_list = parse_file(input_file, process_count);
    fclose(input_file);
    return process_list;
}

int main(int argc, char *argv[]) 
{ 
    int process_count; 
    int quantum_time = 2;

    ProcessType *process_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
    // FCFS
    process_count = 0;
    process_list = initProc(argv[1], &process_count);
    findavgTimeFCFS(process_list, process_count);
    printMetrics(process_list, process_count);
  
    // SJF
    process_count = 0;
    process_list = initProc(argv[1], &process_count);
    findavgTimeSJF(process_list, process_count); 
    printMetrics(process_list, process_count);
  
    // Priority
    process_count = 0; 
    process_list = initProc(argv[1], &process_count);
    findavgTimePriority(process_list, process_count); 
    printMetrics(process_list, process_count);
    
    // RR
    process_count = 0;
    process_list = initProc(argv[1], &process_count);
    findavgTimeRR(process_list, process_count, quantum_time); 
    printMetrics(process_list, process_count);
    
    return 0; 
} 
