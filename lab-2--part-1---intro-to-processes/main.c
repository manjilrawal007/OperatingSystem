#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_ITERATIONS 200
#define BUFFER_SIZE 100

void process_execution(void) {
    pid_t process_id;
    int counter;
    char buffer[BUFFER_SIZE];

    fork(); // Create a new process
    process_id = getpid(); // Get the process ID

    for (counter = 1; counter <= MAX_ITERATIONS; counter++) {
        sprintf(buffer, "Process ID: %d, Counter: %d\n", process_id, counter);
        write(1, buffer, strlen(buffer)); // Write output to standard output
    }
}

int main(void) {
    process_execution();
    return 0;
}
