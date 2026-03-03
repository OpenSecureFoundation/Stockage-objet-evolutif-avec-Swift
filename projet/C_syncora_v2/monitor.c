#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "monitor.h"
#include "swift.h"

#define MAX_FILES 1024

typedef struct {
    char name[256];
    time_t last_modified;
} file_info;

static file_info previous_files[MAX_FILES];
static int previous_count = 0;

void monitor_directory(const char *path) {

    DIR *dir = opendir(path);
    struct dirent *entry;

    if (!dir) {
        perror("Erreur ouverture dossier");
        return;
    }

    file_info current_files[MAX_FILES];
    int current_count = 0;

    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        if (entry->d_type == DT_REG) {

            char fullpath[512];
            snprintf(fullpath, sizeof(fullpath),
                     "%s/%s", path, entry->d_name);

            struct stat file_stat;

            if (stat(fullpath, &file_stat) == 0) {

                strcpy(current_files[current_count].name,
                       entry->d_name);

                current_files[current_count].last_modified =
                       file_stat.st_mtime;

                current_count++;
            }
        }
    }

    closedir(dir);

    // 🔍 Détection création et modification

    for (int i = 0; i < current_count; i++) {

        int found = 0;

        for (int j = 0; j < previous_count; j++) {

            if (strcmp(current_files[i].name,
                       previous_files[j].name) == 0) {

                found = 1;

                if (current_files[i].last_modified !=
                    previous_files[j].last_modified) {

                    // printf("[MODIFICATION] %s\n",
                         //  current_files[i].name);
                         printf("[MODIFICATION] %s\n", current_files[i].name);
                         swift_upload(fullpath);
                }

                break;
            }
        }

        if (!found) {
           // printf("[CREATION] %s\n",
            //      current_files[i].name);
            printf("[CREATION] %s\n", current_files[i].name);
            swift_upload(fullpath);
        }
    }

    // 🔍 Détection suppression

    for (int i = 0; i < previous_count; i++) {

        int found = 0;

        for (int j = 0; j < current_count; j++) {

            if (strcmp(previous_files[i].name,
                       current_files[j].name) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
           // printf("[SUPPRESSION] %s\n",
           //        previous_files[i].name);
           printf("[SUPPRESSION] %s\n", previous_files[i].name);
           swift_delete(previous_files[i].name);
        }
    }

    // 🔄 Sauvegarder état actuel pour prochain scan

    previous_count = current_count;

    for (int i = 0; i < current_count; i++) {
        previous_files[i] = current_files[i];
    }
}
