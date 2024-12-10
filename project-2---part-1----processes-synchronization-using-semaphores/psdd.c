#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>

#define MAX_OFFSPRING 10

void caring_father(int *shared_account, sem_t *sync_semaphore);
void struggling_student(int *shared_account, sem_t *sync_semaphore, int student_id);
void nurturing_mother(int *shared_account, sem_t *sync_semaphore);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <number of parents> <number of students>\n", argv[0]);
        exit(1);
    }

    int num_parents = atoi(argv[1]);
    int num_students = atoi(argv[2]);

    if (num_students > MAX_OFFSPRING) {
        printf("Maximum number of students allowed: %d\n", MAX_OFFSPRING);
        exit(1);
    }

    // Create shared memory for the shared bank account
    int file_descriptor = open("shared_account.txt", O_RDWR | O_CREAT, S_IRWXU);
    if (file_descriptor == -1) {
        perror("Failed to open shared memory file");
        exit(1);
    }

    int initial_value = 0;
    write(file_descriptor, &initial_value, sizeof(int));
    int *shared_account = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (shared_account == MAP_FAILED) {
        perror("Failed to map shared memory");
        exit(1);
    }
    close(file_descriptor);

    *shared_account = 0; // Initialize shared account balance to 0

    // Create semaphore for synchronization
    sem_t *sync_semaphore = sem_open("account_semaphore", O_CREAT, 0644, 1);
    if (sync_semaphore == SEM_FAILED) {
        perror("Failed to initialize semaphore");
        exit(1);
    }

    // Fork processes for parents and students
    pid_t process_id;
    int i;
    for (i = 0; i < num_parents + num_students; i++) {
        process_id = fork();
        if (process_id == 0) {
            srand(time(NULL) + getpid());
            if (i < num_parents) {
                if (i == 0)
                    caring_father(shared_account, sync_semaphore); // First parent is Father
                else
                    nurturing_mother(shared_account, sync_semaphore); // Additional parents are Mother
            } else {
                struggling_student(shared_account, sync_semaphore, i - num_parents);
            }
            exit(0);
        } else if (process_id < 0) {
            perror("Fork failed");
            exit(1);
        }
    }

    // Wait for all processes to terminate
    for (i = 0; i < num_parents + num_students; i++) {
        wait(NULL);
    }

    // Clean up resources
    munmap(shared_account, sizeof(int));
    sem_close(sync_semaphore);
    sem_unlink("account_semaphore");
    unlink("shared_account.txt");

    return 0;
}

void caring_father(int *shared_account, sem_t *sync_semaphore) {
    while (1) {
        sleep(rand() % 5);
        printf("Caring Father: Attempting to Check Balance\n");
        sem_wait(sync_semaphore);

        int local_balance = *shared_account;
        if (rand() % 2 == 0) { // Random number is even
            if (local_balance < 100) {
                int deposit = (rand() % 101) + 1; // Deposit between 1 and 100
                printf("Caring Father: Deposits $%d / Balance = $%d\n", deposit, local_balance + deposit);
                *shared_account += deposit;
            } else {
                printf("Caring Father: Thinks Students have enough Cash ($%d)\n", local_balance);
            }
        } else {
            printf("Caring Father: Last Checked Balance = $%d\n", local_balance);
        }

        sem_post(sync_semaphore);
    }
}

void struggling_student(int *shared_account, sem_t *sync_semaphore, int student_id) {
    while (1) {
        sleep(rand() % 5);
        printf("Struggling Student %d: Attempting to Check Balance\n", student_id);
        sem_wait(sync_semaphore);

        int local_balance = *shared_account;
        if (rand() % 2 == 0) { // Random number is even
            int need = (rand() % 50) + 1; // Need between 1 and 50
            printf("Struggling Student %d needs $%d\n", student_id, need);
            if (local_balance >= need) {
                printf("Struggling Student %d: Withdraws $%d / Balance = $%d\n", student_id, need, local_balance - need);
                *shared_account -= need;
            } else {
                printf("Struggling Student %d: Not Enough Cash ($%d)\n", student_id, local_balance);
            }
        } else {
            printf("Struggling Student %d: Last Checked Balance = $%d\n", student_id, local_balance);
        }

        sem_post(sync_semaphore);
    }
}

void nurturing_mother(int *shared_account, sem_t *sync_semaphore) {
    while (1) {
        sleep(rand() % 10);
        printf("Nurturing Mother: Attempting to Check Balance\n");
        sem_wait(sync_semaphore);

        int local_balance = *shared_account;
        if (local_balance <= 100) {
            int deposit = (rand() % 125) + 1; // Deposit between 1 and 125
            printf("Nurturing Mother: Deposits $%d / Balance = $%d\n", deposit, local_balance + deposit);
            *shared_account += deposit;
        }

        sem_post(sync_semaphore);
    }
}
