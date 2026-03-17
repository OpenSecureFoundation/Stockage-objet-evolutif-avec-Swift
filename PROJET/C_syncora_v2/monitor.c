#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "monitor.h"
#include "swift.h"

void monitor_directory(FolderState *state) {
    // 1. EXTRACTION DU NOM DU CONTENEUR
    // state->folder_name est par ex: "/home/valdez/Documents/Syncora/cours-1"
    // On veut extraire uniquement "cours-1" pour les commandes Swift
    const char *full_folder_path = state->folder_name;
    char *container_name = strrchr(full_folder_path, '/');
    
    if (container_name) {
        container_name++; // On saute le '/' pour avoir "cours-1"
    } else {
        container_name = (char *)full_folder_path;
    }

    DIR *dir = opendir(full_folder_path);
    if (!dir) return;

    file_info current_files[MAX_FILES];
    int current_count = 0;
    struct dirent *entry;

    // 2. LECTURE DU DOSSIER LOCAL
    while ((entry = readdir(dir)) != NULL && current_count < MAX_FILES) {
        if (entry->d_name[0] == '.') continue; 

        char full_file_path[2048];
        snprintf(full_file_path, sizeof(full_file_path), "%s/%s", full_folder_path, entry->d_name);
        
        struct stat st;
        if (stat(full_file_path, &st) == 0 && S_ISREG(st.st_mode)) {
            strncpy(current_files[current_count].name, entry->d_name, 255);
            current_files[current_count].last_modified = st.st_mtime;
            current_count++;
        }
    }
    closedir(dir);

    // 3. DÉTECTION CRÉATION ET MODIFICATION
    for (int i = 0; i < current_count; i++) {
        int found = 0;
        char full_file_path[2048];
        snprintf(full_file_path, sizeof(full_file_path), "%s/%s", full_folder_path, current_files[i].name);

        for (int j = 0; j < state->previous_count; j++) {
            if (strcmp(current_files[i].name, state->previous_files[j].name) == 0) {
                found = 1;
                if (current_files[i].last_modified != state->previous_files[j].last_modified) {
                    printf("[MODIF] %s dans %s\n", current_files[i].name, container_name);
                    // On envoie le nom du conteneur et le chemin local complet
                    swift_upload(container_name, full_file_path); 
                }
                break;
            }
        }

        if (!found) {
            printf("[NEW] %s dans %s\n", current_files[i].name, container_name);
            swift_upload(container_name, full_file_path); 
        }
    }

    // 4. DÉTECTION SUPPRESSION
    for (int i = 0; i < state->previous_count; i++) {
        int found = 0;
        for (int j = 0; j < current_count; j++) {
            if (strcmp(state->previous_files[i].name, current_files[j].name) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("[DEL] %s de %s\n", state->previous_files[i].name, container_name);
            swift_delete(container_name, state->previous_files[i].name);
        }
    }

    // 5. MISE À JOUR DE L'ÉTAT POUR LE PROCHAIN SCAN
    state->previous_count = current_count;
    memcpy(state->previous_files, current_files, sizeof(current_files));
}