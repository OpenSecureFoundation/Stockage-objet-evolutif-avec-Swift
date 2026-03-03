#include <stdio.h>
#include <unistd.h>
#include "monitor.h"

#define SYNC_FOLDER "./Syncora"

int main() {

    printf("=== SYNCORA DETECTION MODE ===\n");

    while (1) {

        monitor_directory(SYNC_FOLDER);

        sleep(5);
    }

    return 0;
}
