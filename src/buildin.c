#include "hashtable.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

void type(char *argv[], size_t argc, hashtable_t *path_cmd)
{
    
    if (argc < 2)
        return;

    size_t i;
    size_t cmd_len;

    for (i = 1; i < argc; ++i)
    {
        cmd_len = strlen(argv[i]);
        /* check if buildin */
        if (cmd_len == 4 && strncmp(argv[i], "exit", 4) == 0 ||
            cmd_len == 4 && strncmp(argv[i], "type", 4) == 0 ||
            cmd_len == 2 && strncmp(argv[i], "cd", 2) == 0   ||
            cmd_len == 3 && strncmp(argv[i], "pwd", 3) == 0  ||
            cmd_len == 5 && strncmp(argv[i], "which", 5) == 0)
            printf("%s: shell build-in command", argv[i]);
        else
        {
            entry_t *entry = ht_get_entry(path_cmd, argv[i]);
            if (entry == NULL)
            {
                printf("%s not found", argv[i]);
                continue;
            }

            printf("%s is %s", argv[i], entry->value);
        }
        if (i < argc - 1)
            printf("\n");
    }
}

void cd(char *argv[], size_t argc)
{

    char *path;
    char *home_env = getenv("HOME");
    
    if (argc < 2)
        path = home_env;
    else
    {
        /* parse (~) into HOME */
        if (strncmp(argv[1], "~", 1) == 0)
            snprintf(path, strlen(argv[1] + 1) + strlen(home_env) + 1, "%s%s", home_env, argv[1] + 1);
        else
            path = argv[1];
    }
    
    char *cwd;
    cwd = getcwd(NULL, 0);

    if (cwd == NULL)
        return;

    if (chdir(path) != 0)
        perror("Couldn't change path");

    if (setenv("OLDPWD", cwd, 1) != 0)
        perror("Error updating env");

    free(cwd);
    cwd = getcwd(NULL, 0);

    if (setenv("PWD", cwd, 1) != 0)
        perror("Error updating env");

    free(cwd);
}

void which(char *argv[], size_t argc, hashtable_t *path_cmd)
{
    if (argc < 2)
        return;

    size_t i;
    size_t cmd_len;

    for (i = 1; i < argc; ++i)
    {
        cmd_len = strlen(argv[i]);

        /* check if buildin */
        if (cmd_len == 4 && strncmp(argv[i], "exit", 4) == 0 ||
            cmd_len == 4 && strncmp(argv[i], "type", 4) == 0 ||
            cmd_len == 2 && strncmp(argv[i], "cd", 2) == 0   ||
            cmd_len == 3 && strncmp(argv[i], "pwd", 3) == 0  ||
            cmd_len == 5 && strncmp(argv[i], "which", 5) == 0)
            printf("%s: shell build-in command", argv[i]);
        else
        {
            entry_t *entry = ht_get_entry(path_cmd, argv[i]);
            if (entry == NULL)
            {
                printf("%s not found", argv[i]);
                continue;
            }

            printf("%s", entry->value);
        }
        
        if (i < argc - 1)
            printf("\n");

    }
}
