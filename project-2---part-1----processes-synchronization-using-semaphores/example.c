#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{
  int fileDescriptor, iterationCount, totalLoops = 10, initialValue = 0, *sharedCounter;
  sem_t *semaphore;

  // Open a file and map it into memory to hold the shared counter
  fileDescriptor = open("shared_log.txt", O_RDWR | O_CREAT, S_IRWXU);
  write(fileDescriptor, &initialValue, sizeof(int));
  sharedCounter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
  close(fileDescriptor);

  /* Create and initialize the semaphore */
  if ((semaphore = sem_open("sync_semaphore", O_CREAT, 0644, 1)) == SEM_FAILED) {
    perror("Semaphore initialization failed");
    exit(1);
  }
 
  if (fork() == 0) { /* Child process */
    for (iterationCount = 0; iterationCount < totalLoops; iterationCount++) {
      printf("Child attempting to enter critical section\n");
      sem_wait(semaphore);
      printf("Child entered critical section: %d\n", (*sharedCounter)++);
      sleep(2);
      printf("Child exiting critical section\n");
      sem_post(semaphore);
      sleep(1);
    }
    exit(0);
  }

  /* Back to parent process */
  for (iterationCount = 0; iterationCount < totalLoops; iterationCount++) {
    printf("Parent attempting to enter critical section\n");
    sem_wait(semaphore);
    printf("Parent entered critical section: %d\n", (*sharedCounter)++);
    sleep(2);
    printf("Parent exiting critical section\n");
    sem_post(semaphore);
    sleep(1);
  }
  exit(0);
}
