/* signal_hello.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Global flag to indicate signal reception
volatile sig_atomic_t signal_received = 0;

// Signal handler function
void signal_handler(int signal_number) {
    printf("Hello, World!\n");
    signal_received = 1;
}

int main(int argc, char *argv[]) {
    // Register the signal handler for SIGALRM
    signal(SIGALRM, signal_handler);

    while (1) {
        alarm(5); // Schedule an alarm signal in 5 seconds
        signal_received = 0; // Reset the signal flag

        // Wait until the signal is received
        while (!signal_received);

        printf("Turing's prediction is correct!\n");
    }

    return 0;
}
