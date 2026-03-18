#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "swift.h"

void swift_upload(const char *container, const char *filepath) {
    char command[4096];
    char *filename = strrchr(filepath, '/');
    filename = (filename) ? filename + 1 : (char *)filepath;

    if (!container || strlen(container) == 0) return;

    snprintf(command, sizeof(command),
             "swift upload %s '%s' --object-name '%s' --changed",
             container, filepath, filename);

    printf("[CLOUD] Upload: %s/%s\n", container, filename);
    system(command);
}

void swift_delete(const char *container, const char *filename) {
    char command[2048];
    if (!container || strlen(container) == 0) return;

    snprintf(command, sizeof(command), "swift delete %s '%s'", container, filename);
    printf("[CLOUD] Delete: %s/%s\n", container, filename);
    system(command);
}

void swift_delete_container(const char *container) {
    char command[1024];
    snprintf(command, sizeof(command), "swift delete %s > /dev/null 2>&1", container);
    printf("[CLOUD] Suppression définitive du conteneur : %s\n", container);
    system(command);
}