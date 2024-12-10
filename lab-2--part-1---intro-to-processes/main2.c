#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define ITERATION_LIMIT 200

void execute_child_process(void);      /* Function prototype for child process */
void execute_parent_process(void);     /* Function prototype for parent process */

int main(void) {
    pid_t process_id;

    process_id = fork();  // Create a new process
    if (process_id == 0) {
        execute_child_process();       // Run child process logic
    } else {
        execute_parent_process();      // Run parent process logic
    }

    return 0;
}

void execute_child_process(void) {
    int counter;

    for (counter = 1; counter <= ITERATION_LIMIT; counter++) {
        printf("   Child Process: Counter = %d\n", counter);
    }
    printf("   *** Child process completed ***\n");
}

void execute_parent_process(void) {
    int counter;

    for (counter = 1; counter <= ITERATION_LIMIT; counter++) {
        printf("Parent Process: Counter = %d\n", counter);
    }
    printf("*** Parent process completed ***\n");
}

}
