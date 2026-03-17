#ifndef MONITOR_H
#define MONITOR_H

#include <time.h>

#define MAX_FILES 1024

// Dans monitor.h
typedef struct {
    char name[256];
    time_t last_modified;
} file_info;

typedef struct {
    char folder_name[256];
    file_info previous_files[MAX_FILES]; // Assure-toi que MAX_FILES est défini (ex: 1024)
    int previous_count;
} FolderState;


// La fonction accepte maintenant un pointeur vers l'état spécifique du dossier
void monitor_directory(FolderState *state);

#endif