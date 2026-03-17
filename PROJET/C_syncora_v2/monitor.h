#ifndef MONITOR_H
#define MONITOR_H

#include <time.h>

#define MAX_FILES 1024
#define MAX_CONTAINERS 50

typedef struct {
    char name[256];
    time_t last_modified;
} file_info;

typedef struct {
    char folder_name[1024]; 
    file_info previous_files[MAX_FILES];
    int previous_count;
} FolderState;

void monitor_directory(FolderState *state);

#endif