// C program to demonstrate two-way communication between processes using pipes 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    int pipe1[2];  // Pipe 1 for sending input from parent to child 
    int pipe2[2];  // Pipe 2 for sending concatenated string from child to parent 
  
    char suffix_child[] = "howard.edu"; 
    char suffix_parent[] = "gobison.org"; 
    char parent_input[100]; 
    char child_input[100]; 
    pid_t process_id; 
  
    if (pipe(pipe1) == -1) 
    { 
        fprintf(stderr, "Pipe creation failed\n"); 
        return 1; 
    } 
    if (pipe(pipe2) == -1) 
    { 
        fprintf(stderr, "Pipe creation failed\n"); 
        return 1; 
    } 
  
    printf("Enter a string to concatenate: ");
    scanf("%s", parent_input); 
  
    process_id = fork(); 
  
    if (process_id < 0) 
    { 
        fprintf(stderr, "Fork operation failed\n"); 
        return 1; 
    } 
  
    // Parent process (P1)
    else if (process_id > 0) 
    { 
        close(pipe1[0]);  // Close read end of first pipe (parent writes)
        close(pipe2[1]);  // Close write end of second pipe (parent reads)
  
        // Write input string to the child and close write end of first pipe
        write(pipe1[1], parent_input, strlen(parent_input) + 1); 
        close(pipe1[1]);
  
        // Wait for child to finish concatenation
        wait(NULL); 
  
        // Read concatenated string from child
        char child_concat_result[200];
        read(pipe2[0], child_concat_result, 200);
        printf("Concatenated string from child: %s\n", child_concat_result);

        // Parent appends "gobison.org" to the string received from child
        strcat(child_concat_result, suffix_parent);
        printf("Concatenated string with parent suffix: %s\n", child_concat_result);
  
        close(pipe2[0]);  // Close read end of second pipe
    } 
  
    // Child process (P2)
    else
    { 
        close(pipe1[1]);  // Close write end of first pipe (child reads)
        close(pipe2[0]);  // Close read end of second pipe (child writes)
  
        // Read string sent by parent
        char temp_concat[100]; 
        read(pipe1[0], temp_concat, 100); 
        close(pipe1[0]);
  
        // Append "howard.edu" to the string
        strcat(temp_concat, suffix_child);
        printf("Concatenated string in child: %s\n", temp_concat);
  
        // Send concatenated string back to parent
        write(pipe2[1], temp_concat, strlen(temp_concat) + 1);
  
        // Prompt user for another input to send back to parent
        printf("Enter another string to send to parent: ");
        scanf("%s", child_input);

        // Send this string back to parent (parent appends "gobison.org")
        write(pipe2[1], child_input, strlen(child_input) + 1);
        close(pipe2[1]);

        exit(0);
    } 
  
    return 0; 
}
