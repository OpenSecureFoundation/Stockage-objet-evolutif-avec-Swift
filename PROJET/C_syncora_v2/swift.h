#ifndef SWIFT_H
#define SWIFT_H

void swift_upload(const char *container, const char *filepath);
void swift_delete(const char *container, const char *filename);
void swift_delete_container(const char *container); 

#endif