#include <stdio.h>
#include <stdlib.h>
#include "swift.h"

#define CONTAINER "syncora-container"

void swift_upload(const char *filepath) {

    char command[2048];

    snprintf(command, sizeof(command),
             "swift upload --changed %s %s",
             CONTAINER, filepath);

    printf("[SYNC] Uploading %s\n", filepath);

    system(command);
}

void swift_delete(const char *filename) {

    char command[2048];

    snprintf(command, sizeof(command),
             "swift delete %s %s",
             CONTAINER, filename);

    printf("[SYNC] Deleting %s from cloud\n", filename);

    system(command);
}
