#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define MAX_LOOPS 30
#define MAX_DELAY 10

void simulate_child_behavior() {
    // Seed the random number generator
    srandom(time(NULL) ^ (getpid() << 16));

    // Determine a random number of iterations between 1 and MAX_LOOPS
    int loop_count = random() % MAX_LOOPS + 1;

    for (int iteration = 0; iteration < loop_count; iteration++) {
        // Print child PID and notify it is going to sleep
        printf("Child Process ID: %d is preparing to sleep!\n", getpid());

        // Sleep for a random interval between 1 and MAX_DELAY seconds
        int sleep_duration = random() % MAX_DELAY + 1;
        sleep(sleep_duration);

        // Print child PID and notify it has awakened
        printf("Child Process ID: %d is awake! Searching for Parent ID: %d.\n", getpid(), getppid());
    }

    // Terminate the child process
    exit(0);
}

int main() {
    pid_t child_proc1, child_proc2;
    int exit_status;

    // Create the first child process
    child_proc1 = fork();

    if (child_proc1 < 0) {
        // Handle fork error
        perror("Failed to create first child process");
        exit(1);
    } else if (child_proc1 == 0) {
        // First child process executes its behavior
        simulate_child_behavior();
    } else {
        // Create the second child process
        child_proc2 = fork();

        if (child_proc2 < 0) {
            // Handle fork error
            perror("Failed to create second child process");
            exit(1);
        } else if (child_proc2 == 0) {
            // Second child process executes its behavior
            simulate_child_behavior();
        } else {
            // Parent process waits for both children to finish execution
            pid_t completed_child;

            completed_child = wait(&exit_status);
            printf("Child Process ID: %d has terminated.\n", completed_child);

            completed_child = wait(&exit_status);
            printf("Child Process ID: %d has terminated.\n", completed_child);

            printf("Parent Process ID: %d has completed waiting for children.\n", getpid());
        }
    }

    return 0;
}
