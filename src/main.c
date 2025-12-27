#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.c"
#include "hashtable.h"

#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    char cmd[CMD_BUFFER_SIZE];
    char *argv[MAX_ARGVS];
    size_t argc;
    hashtable_t *commands;

    commands = ht_create(100);

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


        printf("%s: command not found!\n", cmd);
    } while (TRUE);
    
    return 0;
}

void load_path(hashtable_t *ht)
{
    char *path;
    char *token;

    path = getenv("PATH");
    token = strtok(path, ":");

    while (token != NULL)
    {
        DIR *FD;
        struct dirent *inside_folder;

        FD = opendir(token);

        if (FD == NULL)
        {
            printf("Faild to open dir %s\n", token);
            continue;
        }

        while ((inside_folder = readdir(FD)))
        {
            struct stat buffer;
            int status;

            status = stat(inside_folder->d_name, &buffer);
            if (status == -1)
            {
                fprintf(stderr, "Error: Failed to stat item - %s", strerror(errno));
                continue;
            }
            
            char full_path[strlen(token) + strlen(inside_folder->d_name)];
            if (S_ISREG(buffer.st_mode) && access(full_path, X_OK) == 0)
            {
                /* exec the file using execve() */
            }
        }

        closedir(FD);
    }
}
