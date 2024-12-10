#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  ChildProcess(int []);

int  main(int  argc, char *argv[])
{
     int    sharedMemoryID;
     int    *sharedMemoryPTR;
     pid_t  processID;
     int    childStatus;

     if (argc != 5) {
          printf("Usage: %s value1 value2 value3 value4\n", argv[0]);
          exit(1);
     }

     sharedMemoryID = shmget(IPC_PRIVATE, 4 * sizeof(int), IPC_CREAT | 0666);
     if (sharedMemoryID < 0) {
          printf("*** Error: Unable to allocate shared memory ***\n");
          exit(1);
     }
     printf("Server: Shared memory of four integers allocated successfully...\n");

     sharedMemoryPTR = (int *) shmat(sharedMemoryID, NULL, 0);
     if (*sharedMemoryPTR == -1) {
          printf("*** Error: Unable to attach shared memory ***\n");
          exit(1);
     }
     printf("Server: Shared memory attached successfully...\n");

     sharedMemoryPTR[0] = atoi(argv[1]);
     sharedMemoryPTR[1] = atoi(argv[2]);
     sharedMemoryPTR[2] = atoi(argv[3]);
     sharedMemoryPTR[3] = atoi(argv[4]);
     printf("Server: Filled shared memory with values %d %d %d %d\n",
            sharedMemoryPTR[0], sharedMemoryPTR[1], sharedMemoryPTR[2], sharedMemoryPTR[3]);

     printf("Server: Forking a child process...\n");
     processID = fork();
     if (processID < 0) {
          printf("*** Error: Fork operation failed ***\n");
          exit(1);
     }
     else if (processID == 0) {
          ChildProcess(sharedMemoryPTR);
          exit(0);
     }

     wait(&childStatus);
     printf("Server: Child process has completed execution...\n");
     shmdt((void *) sharedMemoryPTR);
     printf("Server: Detached shared memory...\n");
     shmctl(sharedMemoryID, IPC_RMID, NULL);
     printf("Server: Removed shared memory...\n");
     printf("Server: Exiting...\n");
     exit(0);
}

void  ChildProcess(int  memoryArray[])
{
     printf("   Child: Process started\n");
     printf("   Child: Found values %d %d %d %d in shared memory\n",
                memoryArray[0], memoryArray[1], memoryArray[2], memoryArray[3]);
     printf("   Child: Process exiting\n");
}
