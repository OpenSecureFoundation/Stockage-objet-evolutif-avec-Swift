#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "init.h"
#include "monitor.h"

#define MAX_CONTAINERS 50 

int main() {
    setup_containers();

    // ALLOCATION DYNAMIQUE au lieu de FolderState states[50]
    FolderState *states = malloc(sizeof(FolderState) * MAX_CONTAINERS);
    if (states == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire.\n");
        return 1;
    }
    
    // Initialiser la mémoire à zéro
    memset(states, 0, sizeof(FolderState) * MAX_CONTAINERS);

    int count = 0;
    FILE *fp = popen("swift list", "r");
    if (!fp) {
        free(states);
        return 1;
    }

    char name[256];
    while (fgets(name, sizeof(name), fp) && count < MAX_CONTAINERS) {
        name[strcspn(name, "\n")] = 0;
        if (strlen(name) > 0) {
            strncpy(states[count].folder_name, name, 255);
            states[count].previous_count = 0; 
            count++;
        }
    }
    pclose(fp);

    printf("=== SYNCORA V2 : SURVEILLANCE DE %d DOSSIERS ===\n", count);

    while (1) {
        for (int i = 0; i < count; i++) {
            monitor_directory(&states[i]);
        }
        sleep(5);
    }

    free(states); // Toujours libérer ce qu'on a alloué
    return 0;
}