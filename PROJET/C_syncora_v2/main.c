#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "init.h"
#include "monitor.h"

int main() {
    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Erreur : Impossible de trouver la variable HOME.\n");
        return 1;
    }

    // Initialisation physique des dossiers dans Documents/Syncora
    setup_containers();

    // Allocation dynamique (Heap) pour éviter le Segfault
    FolderState *states = malloc(sizeof(FolderState) * MAX_CONTAINERS);
    if (!states) {
        perror("Erreur allocation mémoire");
        return 1;
    }
    memset(states, 0, sizeof(FolderState) * MAX_CONTAINERS);

    FILE *fp = popen("swift list", "r");
    if (!fp) {
        perror("Erreur swift list");
        free(states);
        return 1;
    }

    int count = 0;
    char name[256];
    while (fgets(name, sizeof(name), fp) && count < MAX_CONTAINERS) {
        name[strcspn(name, "\n")] = 0;
        if (strlen(name) > 0) {
            // Utilisation sécurisée de sizeof pour folder_name (1024 octets)
            snprintf(states[count].folder_name, sizeof(states[count].folder_name), 
                     "%s/Documents/Syncora/%s", home, name);
            states[count].previous_count = 0;
            count++;
        }
    }
    pclose(fp);

    printf("=== SYNCORA : %d DOSSIERS EN SURVEILLANCE ===\n", count);
    printf("[INFO] Racine : %s/Documents/Syncora\n", home);

    while (1) {
        for (int i = 0; i < count; i++) {
            monitor_directory(&states[i]);
        }
        sleep(5); // Cycle de 5 secondes
    }

    free(states);
    return 0;
}