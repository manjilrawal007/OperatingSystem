#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * This program demonstrates piping in Unix by executing
 * the command "cat scores | grep Lakers". The parent process
 * runs "cat scores" and sends the output through a pipe,
 * while the child process runs "grep Lakers" to filter the input.
 * Note: The parent does not wait for the child process to complete.
 */

int main(int argc, char **argv)
{
  int pipe_fds[2];
  int process_id;

  char *cat_command[] = {"cat", "scores", NULL};
  char *grep_command[] = {"grep", "Lakers", NULL};

  // Create a pipe to establish communication between processes
  pipe(pipe_fds);

  process_id = fork();

  if (process_id == 0)
    {
      // Child process: handles the "grep Lakers" command

      // Redirect the standard input to the read end of the pipe
      dup2(pipe_fds[0], 0);

      // Close the write end of the pipe as it is not used by the child
      close(pipe_fds[1]);

      // Execute the "grep" command
      execvp("grep", grep_command);
    }
  else
    {
      // Parent process: handles the "cat scores" command

      // Redirect the standard output to the write end of the pipe
      dup2(pipe_fds[1], 1);

      // Close the read end of the pipe as it is not used by the parent
      close(pipe_fds[0]);

      // Execute the "cat" command
      execvp("cat", cat_command);
    }
}
