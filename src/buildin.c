#include "hashtable.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

void type(char *argv[], size_t argc, hashtable_t *path_cmd)
{
    if (argc < 2)
        return;

    size_t i;

    for (i = 1; i < argc; ++i)
    {
        if (strncmp(argv[i], "exit", 4) == 0 ||
            strncmp(argv[i], "type", 4) == 0 ||
            strncmp(argv[i], "cd", 2) == 0)
            printf("%s is a shell buildin", argv[i]);
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
    
}
