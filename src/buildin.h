#ifndef L_BUILDIN_H
#define L_BUILDIN_H
#include "hashtable.h"
#include <stdio.h>

void type(char *argv[], size_t argc, hashtable_t *path_cmd);
void cd(char *argv[], size_t argc);
void which(char *argv[], size_t argc, hashtable_t *path_cmd);

#endif
