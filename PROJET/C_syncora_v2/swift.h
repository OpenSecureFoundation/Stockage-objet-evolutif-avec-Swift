#ifndef SWIFT_H
#define SWIFT_H

// On passe le conteneur ET le chemin du fichier
void swift_upload(const char *container, const char *filepath);
void swift_delete(const char *container, const char *filename);

#endif