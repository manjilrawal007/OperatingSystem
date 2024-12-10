#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t signal_flag = 0;
time_t program_start_time;

void alarm_signal_handler(int signal_number)
{
    printf("Hello, World!\n");
    signal_flag = 1;
}

void interrupt_signal_handler(int signal_number)
{
    time_t program_end_time;
    time(&program_end_time);
    printf("\nTotal runtime: %ld seconds\n", program_end_time - program_start_time);
    exit(0);
}

int main(int argc, char *argv[])
{
    time(&program_start_time); // Record the start time

    signal(SIGALRM, alarm_signal_handler); // Register the alarm signal handler
    signal(SIGINT, interrupt_signal_handler); // Register the interrupt signal handler

    while (1) {
        alarm(1); // Set an alarm to go off every 1 second
        signal_flag = 0; // Reset the signal flag

        // Busy wait until the alarm signal is received
        while (!signal_flag);
    }

    return 0;
}
