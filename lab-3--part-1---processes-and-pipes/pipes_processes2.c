#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers". In this implementation,
 * the parent doesn't wait for the child to finish, so the command prompt
 * may reappear before the child terminates.
 */

int main(int argc, char **argv)
{
  int pipe_fds[2];
  int process_id;

  char *cat_command[] = {"cat", "scores", NULL};
  char *grep_command[] = {"grep", "Lakers", NULL};

  // Create a pipe (file descriptors go in pipe_fds[0] and pipe_fds[1])

  pipe(pipe_fds);

  process_id = fork();

  if (process_id == 0)
    {
      // Child process: handles "grep Lakers"

      // Replace standard input with the input end of the pipe

      dup2(pipe_fds[0], 0);

      // Close unused write end of the pipe

      close(pipe_fds[1]);

      // Execute the grep command

      execvp("grep", grep_command);
    }
  else
    {
      // Parent process: handles "cat scores"

      // Replace standard output with the output end of the pipe

      dup2(pipe_fds[1], 1);

      // Close unused read end of the pipe

      close(pipe_fds[0]);

      // Execute the cat command

      execvp("cat", cat_command);
    }
}
