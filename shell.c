#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_CMDS 10
#define MAX_ARGS 100
#define MAX_LEN 1024

int main()
{
    while(1)
    {
        char command[MAX_LEN];

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

        // ---------------- PARSE INTO COMMANDS ----------------

        char *cmds[MAX_CMDS][MAX_ARGS];
        int argc[MAX_CMDS];

        for(int i = 0; i < MAX_CMDS; i++)
            argc[i] = 0;

        int cmd_index = 0;

        char *token = strtok(command, " ");

        while(token != NULL)
        {
            if(strcmp(token, "|") == 0)
            {
                cmds[cmd_index][argc[cmd_index]] = NULL;
                cmd_index++;
            }
            else
            {
                cmds[cmd_index][argc[cmd_index]++] = token;
            }

            token = strtok(NULL, " ");
        }

        cmds[cmd_index][argc[cmd_index]] = NULL;

        int num_cmds = cmd_index + 1;
        int num_pipes = num_cmds - 1;

        if(num_cmds == 0)
            continue;

        // ---------------- CREATE PIPES ----------------

        int pipes[num_pipes][2];

        for(int i = 0; i < num_pipes; i++)
        {
            if(pipe(pipes[i]) < 0)
            {
                perror("pipe failed");
                exit(1);
            }
        }

        // ---------------- FORK CHILDREN ----------------

        pid_t pid[num_cmds];

        for(int i = 0; i < num_cmds; i++)
        {
            pid[i] = fork();

            if(pid[i] < 0)
            {
                perror("fork failed");
                exit(1);
            }

            if(pid[i] == 0)
            {
                // -------- input from previous pipe --------
                if(i > 0)
                {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                }

                // -------- output to next pipe --------
                if(i < num_cmds - 1)
                {
                    dup2(pipes[i][1], STDOUT_FILENO);
                }

                // -------- close all pipes --------
                for(int j = 0; j < num_pipes; j++)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                // -------- execute command --------
                execvp(cmds[i][0], cmds[i]);

                perror("execvp failed");
                exit(1);
            }
        }

        // ---------------- PARENT CLEANUP ----------------

        for(int i = 0; i < num_pipes; i++)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        for(int i = 0; i < num_cmds; i++)
        {
            waitpid(pid[i], NULL, 0);
        }
    }

    return 0;
}
