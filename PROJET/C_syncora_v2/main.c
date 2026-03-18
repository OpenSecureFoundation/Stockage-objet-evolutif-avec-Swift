#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "init.h"
#include "monitor.h"
#include "swift.h"

int refresh_sync_state(FolderState *states, int *current_count, const char *home) {
    char base_path[1024];
    snprintf(base_path, sizeof(base_path), "%s/Documents/Syncora", home);

    //ÉTAPE 1 : LOCAL -> DELETE CLOUD (Suppression prioritaire)
    // Si le dossier n'existe plus sur le PC, on tue le conteneur sur Swift AVANT toute chose
    FILE *fp_del = popen("swift list", "r");
    if (fp_del) {
        char remote_name[256];
        while (fgets(remote_name, sizeof(remote_name), fp_del)) {
            remote_name[strcspn(remote_name, "\n")] = 0;
            if (strlen(remote_name) == 0) continue;

            char local_p[2048];
            snprintf(local_p, sizeof(local_p), "%s/%s", base_path, remote_name);
            
            struct stat st;
            if (stat(local_p, &st) == -1) {
                // Le dossier n'est plus là ! On supprime sur Swift
                swift_delete_container(remote_name);
            }
        }
        pclose(fp_del);
    }

    //ÉTAPE 2 : CLOUD -> LOCAL (Création des nouveaux conteneurs distants) 
    FILE *fp_list = popen("swift list", "r");
    if (fp_list) {
        char name[256];
        while (fgets(name, sizeof(name), fp_list)) {
            name[strcspn(name, "\n")] = 0;
            if (strlen(name) == 0) continue;

            char full_p[2048];
            snprintf(full_p, sizeof(full_p), "%s/%s", base_path, name);
            
            struct stat st = {0};
            if (stat(full_p, &st) == -1) {
                mkdir(full_p, 0755);
                printf("[SYNC-IN] Nouveau conteneur distant créé localement : %s\n", name);
            }
        }
        pclose(fp_list);
    }

    //ÉTAPE 3 : LOCAL -> CLOUD (Nouveaux dossiers créés par l'utilisateur)
    DIR *d = opendir(base_path);
    if (d) {
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_name[0] == '.') continue;
            char full_p[2048];
            snprintf(full_p, sizeof(full_p), "%s/%s", base_path, dir->d_name);
            struct stat st;
            if (stat(full_p, &st) == 0 && S_ISDIR(st.st_mode)) {
                char cmd[2560]; 
                snprintf(cmd, sizeof(cmd), "swift post '%s' > /dev/null 2>&1", dir->d_name);
                system(cmd);
            }
        }
        closedir(d);
    }

    //ÉTAPE 4 : ACTUALISATION DE LA LISTE DE SURVEILLANCE 
    int new_count = 0;
    FILE *fp_final = popen("swift list", "r");
    if (fp_final) {
        char name[256];
        while (fgets(name, sizeof(name), fp_final) && new_count < MAX_CONTAINERS) {
            name[strcspn(name, "\n")] = 0;
            if (strlen(name) > 0) {
                char full_p[2048];
                snprintf(full_p, sizeof(full_p), "%s/%s", base_path, name);
                
                int found = -1;
                for(int i=0; i < *current_count; i++) {
                    if(strcmp(states[i].folder_name, full_p) == 0) {
                        found = i;
                        break;
                    }
                }
                if (found == -1) {
                    strncpy(states[new_count].folder_name, full_p, 1023);
                    states[new_count].previous_count = 0;
                } else if (new_count != found) {
                    states[new_count] = states[found];
                }
                new_count++;
            }
        }
        pclose(fp_final);
    }
    *current_count = new_count;
    return 0;
}

int main() {
    char *home = getenv("HOME");
    if (!home) return 1;

    setup_containers();

    FolderState *states = malloc(sizeof(FolderState) * MAX_CONTAINERS);
    if (!states) return 1;
    memset(states, 0, sizeof(FolderState) * MAX_CONTAINERS);
    int count = 0;

    printf("=== SYNCORA V2 : DYNAMIQUE ET BIDIRECTIONNEL ===\n");

    while (1) {
        refresh_sync_state(states, &count, home);
        for (int i = 0; i < count; i++) {
            monitor_directory(&states[i]);
        }
        sleep(5); 
    }

    free(states);
    return 0;
}