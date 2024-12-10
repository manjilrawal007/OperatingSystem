#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define SHARED_MEMORY_KEY 9876 // Key for shared memory

void ParentRoutine(int *AccountBalance, int *TurnFlag); // Parent (Dear Old Dad)
void ChildRoutine(int *AccountBalance, int *TurnFlag);  // Child (Poor Student)

int main() {
    int shared_mem_id;
    int *shared_mem_ptr;
    pid_t child_pid;
    int child_status;

    // Create shared memory segment for two integers (AccountBalance and TurnFlag)
    shared_mem_id = shmget(SHARED_MEMORY_KEY, 2 * sizeof(int), IPC_CREAT | 0666);
    if (shared_mem_id < 0) {
        printf("*** Error: Failed to create shared memory ***\n");
        exit(1);
    }
    printf("Shared memory successfully created.\n");

    // Attach to the shared memory segment
    shared_mem_ptr = (int *) shmat(shared_mem_id, NULL, 0);
    if (shared_mem_ptr == (int *) -1) {
        printf("*** Error: Failed to attach shared memory ***\n");
        exit(1);
    }
    printf("Shared memory attached.\n");

    // Initialize shared variables
    shared_mem_ptr[0] = 0; // AccountBalance starts at $0
    shared_mem_ptr[1] = 0; // TurnFlag starts at 0 (Parent's turn)

    // Fork a child process
    child_pid = fork();
    if (child_pid < 0) {
        printf("*** Error: Fork operation failed ***\n");
        exit(1);
    }

    if (child_pid == 0) {
        // Child process
        ChildRoutine(&shared_mem_ptr[0], &shared_mem_ptr[1]);
        exit(0);
    } else {
        // Parent process
        ParentRoutine(&shared_mem_ptr[0], &shared_mem_ptr[1]);
        
        // Wait for the child process to complete
        wait(&child_status);

        // Detach and remove the shared memory segment
        shmdt(shared_mem_ptr);
        shmctl(shared_mem_id, IPC_RMID, NULL);
        printf("Shared memory detached and removed.\n");
    }

    return 0;
}

void ParentRoutine(int *AccountBalance, int *TurnFlag) {
    int balance, deposit_amount;
    srand(time(NULL)); // Seed the random number generator

    for (int i = 0; i < 25; i++) {
        sleep(rand() % 6); // Sleep for a random duration between 0-5 seconds

        balance = *AccountBalance;
        while (*TurnFlag != 0); // Wait for Parent's turn

        // Deposit money if balance is $100 or less
        if (balance <= 100) {
            deposit_amount = rand() % 101; // Generate a random deposit between $0 and $100

            if (deposit_amount % 2 == 0) { // Deposit only if the amount is even
                balance += deposit_amount;
                printf("Dear Old Dad: Deposits $%d / Balance = $%d\n", deposit_amount, balance);
            } else {
                printf("Dear Old Dad: No money to deposit\n");
            }
        } else {
            printf("Dear Old Dad: Student has enough cash ($%d)\n", balance);
        }

        // Update shared memory and pass turn to child
        *AccountBalance = balance;
        *TurnFlag = 1; // Set TurnFlag to 1 for the child
    }
}

void ChildRoutine(int *AccountBalance, int *TurnFlag) {
    int balance, withdrawal_amount;
    srand(time(NULL) + 1); // Seed the random number generator with a different value for the child

    for (int i = 0; i < 25; i++) {
        sleep(rand() % 6); // Sleep for a random duration between 0-5 seconds

        balance = *AccountBalance;
        while (*TurnFlag != 1); // Wait for Child's turn

        // Attempt to withdraw money
        withdrawal_amount = rand() % 51; // Generate a random withdrawal between $0 and $50
        printf("Poor Student needs $%d\n", withdrawal_amount);

        if (withdrawal_amount <= balance) {
            balance -= withdrawal_amount;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", withdrawal_amount, balance);
        } else {
            printf("Poor Student: Insufficient funds ($%d)\n", balance);
        }

        // Update shared memory and pass turn to parent
        *AccountBalance = balance;
        *TurnFlag = 0; // Set TurnFlag to 0 for the parent
    }
}
