#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.c"
#include "hashtable.h"
#include "bin/buildin.c"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    char cmd[CMD_BUFFER_SIZE];
    char *argv[MAX_ARGVS];
    size_t argc;
    hashtable_t *commands;
    entry_t *cmd_path;

    commands = ht_create(50);
    load_path(commands);

    do
    {
        printf("$ ");
        fflush(stdout);

        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd) - 1] = '\0';

        /* tokenize input argv */
        argc = 0;
        char *token = strtok(cmd, " ");

        while (token != NULL && argc < MAX_ARGVS - 1)
        {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;

        if (argc == 0)
            continue;
        
        if (strncmp(cmd, "exit", 4) == 0)
            break;
        if (strncmp(cmd, "echo", 4) == 0)
            echo(argv, argc);

        cmd_path = ht_get_entry(commands, cmd);

        if (cmd_path != NULL)
        {
            pid_t pid = fork();

            if (pid == 0)
            {
                execve(strdup(cmd_path->value), argv, NULL);
                printf("%s: command not found!\n", cmd);
                _exit(127);
            }
        }
        else
            printf("%s: command not found!\n", cmd);
    } while (TRUE);
    
    clear_ht(commands);
    return 0;
}

void load_path(hashtable_t *ht)
{
    char *path;
    char *token;

    path = strdup(getenv("PATH"));
    token = strtok(path, ":");

    while (token)
    {
        DIR *FD;
        struct dirent *inside_folder;

        FD = opendir(token);

        if (FD == NULL)
        {
            token = strtok(NULL, ":");
            continue;
        }

        while ((inside_folder = readdir(FD)))
        {
            if (strcmp(inside_folder->d_name, ".") == 0 ||
                strcmp(inside_folder->d_name, "..") == 0)
                continue;

            struct stat buffer;
            int status;
            size_t token_size = strlen(token);
            size_t d_name_size = strlen(inside_folder->d_name);
            char full_path[token_size + d_name_size + 2];
            
            snprintf(full_path, sizeof(full_path), "%s/%s", token, inside_folder->d_name);

            status = stat(full_path, &buffer);
            if (status == -1)
            {
                fprintf(stderr, "Error: Failed to stat item (%s) - %s\n", full_path, strerror(errno));
                continue;
            }

            printf("file: %s\n", inside_folder->d_name);

            if (S_ISREG(buffer.st_mode) && access(full_path, X_OK) == 0)
            {
                ht_set(ht, inside_folder->d_name, strdup(full_path));
            }
        }

        token = strtok(NULL, ":");
        closedir(FD);
        
    }

    free(path);
}
