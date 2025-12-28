#ifndef MAIN_H
#define MAIN_H
#include "hashtable.h"

#define TRUE                    (1)
#define FALSE                   (0)
#define CMD_BUFFER_SIZE         (1024)
#define MAX_ARGVS               (50)
#define PATH_MAX_SIZE           (50)

void load_path(hashtable_t *ht);

#endif
