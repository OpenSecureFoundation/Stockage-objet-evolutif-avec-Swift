#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "init.h"

void setup_containers() {
    FILE *fp;
    char container_name[256];

    // On récupère la liste des conteneurs via Swift
    fp = popen("swift list", "r");
    if (fp == NULL) {
        perror("Erreur lors de l'appel à swift list");
        return;
    }

    printf("\n--- Initialisation des dossiers ---\n");
    while (fgets(container_name, sizeof(container_name), fp) != NULL) {
        // Enlever le saut de ligne (\n) pour avoir un nom de dossier propre
        container_name[strcspn(container_name, "\n")] = 0;

        if (strlen(container_name) > 0) {
            struct stat st = {0};
            // Vérifier si le dossier existe, sinon le créer
            if (stat(container_name, &st) == -1) {
                if (mkdir(container_name, 0755) == 0) {
                    printf("[INIT] Dossier créé : %s\n", container_name);
                } else {
                    perror("[ERREUR] Impossible de créer le dossier");
                }
            } else {
                printf("[INIT] Dossier déjà existant : %s\n", container_name);
            }
        }
    }
    printf("--- Fin de l'initialisation ---\n\n");
    pclose(fp);
}