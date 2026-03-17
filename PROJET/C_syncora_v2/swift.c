#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "swift.h"

void swift_upload(const char *container, const char *filepath) {
    char command[2048];
    
    // On extrait le nom du fichier (ex: photo.jpg) du chemin complet
    char *filename = strrchr(filepath, '/');
    if (filename) filename++; else filename = (char *)filepath;

    // SECURITÉ : Si le container est vide ou nul, on arrête
    if (!container || strlen(container) == 0) {
        fprintf(stderr, "[ERREUR] Nom de conteneur invalide\n");
        return;
    }

    // Commande Swift : on utilise -S pour la simplicité
    // On force l'objet à s'appeler uniquement 'filename'
    snprintf(command, sizeof(command),
             "swift upload %s '%s' --object-name '%s'",
             container, filepath, filename);

    printf("[SYNC-CLOUD] Uploading to %s: %s\n", container, filename);
    system(command);
}

void swift_delete(const char *container, const char *filename) {
    char command[2048];
    if (!container || strlen(container) == 0) return;

    snprintf(command, sizeof(command), "swift delete %s '%s'", container, filename);
    printf("[SYNC-CLOUD] Deleting from %s: %s\n", container, filename);
    system(command);
}