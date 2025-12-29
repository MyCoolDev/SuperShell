#include "main.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "hashtable.c"
#include "hashtable.h"
#include "buildin.c"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    char *home;
    char *username;
    char *pwd;
    char host_name[HOST_NAME_MAX + 1];
    int temp = gethostname(host_name, sizeof(host_name));

    if (temp == -1)
    {
        perror("gethostname");
        return 1;
    }

    char cmd[CMD_BUFFER_SIZE];
    char *argv[MAX_ARGVS];
    size_t argc;
    hashtable_t *commands;
    entry_t *cmd_path;
    commands = ht_create(50);
    load_path(commands);

    do
    {
        username = getenv("USER");
        pwd = getenv("PWD");
        home = replace_home_with_tilde(pwd);
        
        printf("\n%s%s %s@%s at %s\n", TL, HB, username, host_name, home);
        printf("%s%s> ", BL, HB);
        fflush(stdout);
        free(home);

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
        
        if (argc == 0)
            continue;
        
        argv[argc] = NULL;

        size_t cmd_s = strlen(argv[0]);

        if (cmd_s == 4 && strncmp(argv[0], "exit", cmd_s) == 0)
            break;
        if (cmd_s == 4 && strncmp(cmd, "type", cmd_s) == 0)
        {
            type(argv, argc, commands);
            continue;
        }

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
            
            waitpid(pid, NULL, 0);
        }
        else
            printf("%s: command not found!", cmd);
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
                continue;

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

char *replace_home_with_tilde(char *path)
{
    char *home;
    size_t home_len;
    char *new_path;
    size_t new_path_len;

    home = getenv("HOME");

    if (home == NULL)
        return path;

    home_len = strlen(home);

    if (strncmp(path, home, home_len) != 0)
        return path;

    if (path[home_len] == '\0')
        return "~";

    if (path[home_len] != '/')
        return path;

    new_path_len = strlen(path + home_len);
    new_path = (char *)malloc(new_path_len + 2);

    if (!new_path)
        return NULL;

    strcpy(new_path + 1, path + home_len);
    new_path[0] = '~';

    return new_path;
}
