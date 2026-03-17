#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "init.h"

void setup_containers() {
    FILE *fp;
    char container_name[256];
    char base_path[512];
    
    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "Erreur : Impossible de trouver le dossier personnel.\n");
        return;
    }

    snprintf(base_path, sizeof(base_path), "%s/Documents/Syncora", home);

    mkdir(base_path, 0755);

    fp = popen("swift list", "r");
    if (fp == NULL) return;

    printf("--- Initialisation : %s ---\n", base_path);
    while (fgets(container_name, sizeof(container_name), fp) != NULL) {
        container_name[strcspn(container_name, "\n")] = 0;

        if (strlen(container_name) > 0) {
            char full_folder_path[1024];
            snprintf(full_folder_path, sizeof(full_folder_path), "%s/%s", base_path, container_name);

            struct stat st = {0};
            if (stat(full_folder_path, &st) == -1) {
                mkdir(full_folder_path, 0755);
                printf("[INIT] Dossier créé : %s\n", container_name);
            }
        }
    }
    pclose(fp);
}