#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h> // Pour chdir
#include "init.h"
#include "swift.h"

void setup_containers() {
    char *home = getenv("HOME");
    if (!home) return;

    char base_path[1024];
    snprintf(base_path, sizeof(base_path), "%s/Documents/Syncora", home);
    
    // Créer le répertoire racine Syncora s'il n'existe pas
    mkdir(base_path, 0755);

    // Lister les conteneurs sur Swift
    FILE *fp = popen("swift list", "r");
    if (!fp) return;

    char container_name[256];
    char current_dir[1024];
    getcwd(current_dir, sizeof(current_dir)); // Sauvegarder le répertoire actuel

    while (fgets(container_name, sizeof(container_name), fp) != NULL) {
        container_name[strcspn(container_name, "\n")] = 0;

        // On ignore les conteneurs de versions et les lignes vides
        if (strlen(container_name) > 0 && strstr(container_name, "_versions") == NULL) {
            char full_path[2048];
            snprintf(full_path, sizeof(full_path), "%s/%s", base_path, container_name);
            
            // 1. Créer le dossier local
            mkdir(full_path, 0755);
            printf("[INIT] Synchronisation du conteneur : %s\n", container_name);

            // 2. Aller dans ce dossier pour télécharger les fichiers
            if (chdir(full_path) == 0) {
                char download_cmd[512];
                // On télécharge tout le contenu du conteneur Swift vers le local
                snprintf(download_cmd, sizeof(download_cmd), "swift download %s > /dev/null 2>&1", container_name);
                system(download_cmd);
                
                // Revenir au répertoire de base pour la suite de la boucle
                chdir(current_dir);
            }
        }
    }
    pclose(fp);
}