#include <stdio.h>
#include <stdlib.h>
#include "swift.h"
#include <string.h>

// Dans swift.c, modification optionnelle mais recommandée
void swift_upload(const char *container, const char *filepath) {
    char command[2048];
    // On extrait le nom de fichier du chemin (après le dernier /)
    char *filename = strrchr(filepath, '/');
    if (filename) filename++; else filename = (char *)filepath;

    // --object-name force le nom sur le cloud sans le chemin local
    snprintf(command, sizeof(command),
             "swift upload %s '%s' --object-name '%s'",
             container, filepath, filename);

    system(command);
}

void swift_delete(const char *container, const char *filename) {
    char command[2048];

    // Construction de la commande : swift delete conteneur 'nom_du_fichier'
    snprintf(command, sizeof(command),
             "swift delete %s '%s'",
             container, filename);

    printf("[SYNC-CLOUD] Deleting from %s: %s\n", container, filename);

    system(command);
}