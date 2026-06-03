#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    while(1)
    {
        char command[1024];

        printf("myshell> ");

        if(fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        command[strcspn(command, "\n")] = '\0';

        if(strcmp(command, "exit") == 0)
        {
            break;
        }

        // ---------------- PARSING ----------------
        char *argv1[100];
        char *argv2[100];
        int argc1 = 0, argc2 = 0;

        int is_pipe = 0;

        char *token = strtok(command, " ");

        while(token != NULL)
        {
            if(strcmp(token, "|") == 0)
            {
                is_pipe = 1;
                token = strtok(NULL, " ");
                continue;
            }

            if(!is_pipe)
            {
                argv1[argc1++] = token;
            }
            else
            {
                argv2[argc2++] = token;
            }

            token = strtok(NULL, " ");
        }

        argv1[argc1] = NULL;
        argv2[argc2] = NULL;

        // ---------------- NO PIPE CASE ----------------
        if(argc1 == 0)
            continue;

        if(!is_pipe)
        {
            pid_t pid = fork();

            if(pid < 0)
            {
                perror("fork failed");
                continue;
            }

            if(pid == 0)
            {
                execvp(argv1[0], argv1);
                perror("exec failed");
                exit(1);
            }
            else
            {
                wait(NULL);
            }

            continue;
        }

        // ---------------- PIPE CASE ----------------
        int fd[2];

        if(pipe(fd) < 0)
        {
            perror("pipe failed");
            continue;
        }

        // -------- child 1 (left command) --------
        pid_t pid1 = fork();

        if(pid1 < 0)
        {
            perror("fork failed");
            continue;
        }

        if(pid1 == 0)
        {
            dup2(fd[1], STDOUT_FILENO);

            close(fd[0]);
            close(fd[1]);

            execvp(argv1[0], argv1);

            perror("execvp failed");
            exit(1);
        }

        // -------- child 2 (right command) --------
        pid_t pid2 = fork();

        if(pid2 < 0)
        {
            perror("fork failed");
            continue;
        }

        if(pid2 == 0)
        {
            dup2(fd[0], STDIN_FILENO);

            close(fd[0]);
            close(fd[1]);

            execvp(argv2[0], argv2);

            perror("execvp failed");
            exit(1);
        }

        // ---------------- parent ----------------
        close(fd[0]);
        close(fd[1]);

        wait(NULL);
        wait(NULL);
    }

    return 0;
}