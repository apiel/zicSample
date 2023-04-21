#ifndef _FS_H_
#define _FS_H_

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h> // mkdir
#include <sys/types.h> // mkdir

struct dirent* myReaddir(DIR* x)
{
    struct dirent* directory;
    directory = readdir(x);
    if (directory != NULL && (strcmp(directory->d_name, ".") == 0 || strcmp(directory->d_name, "..") == 0)) {
        return myReaddir(x);
    }
    return directory;
}

int8_t myStrcmp(const char* __s1, const char* __s2)
{
    int res = strcmp(__s1, __s2);
    if (res == 0) {
        return 0;
    }
    return res > 0 ? 1 : -1;
}

bool nextFile(char* filename, const char* folder, const char* current, int8_t direction, uint8_t len = 255)
{
    bool next = false;
    DIR* x = opendir(folder);
    if (x != NULL) {
        struct dirent* directory;

        char cur[255];
        strncpy(cur, current, len);
        strncpy(filename, cur, len);
        while ((directory = myReaddir(x)) != NULL) {
            if (myStrcmp(directory->d_name, cur) == direction
                && (strcmp(filename, cur) == 0 || myStrcmp(directory->d_name, filename) == direction * -1)) {
                strncpy(filename, directory->d_name, len);
                next = true;
            }
        }
        closedir(x);
    }
    return next;
}

bool firstFile(char* filename, const char* folder, uint8_t len = 255)
{
    bool initialized = false;
    DIR* x = opendir(folder);
    if (x != NULL) {
        struct dirent* directory;
        while ((directory = myReaddir(x)) != NULL) {
            // printf("%s <> %s = %d\n", directory->d_name, filename, myStrcmp(directory->d_name, filename));
            if (!initialized || myStrcmp(directory->d_name, filename) == -1) {
                strncpy(filename, directory->d_name, len);
                initialized = true;
            }
        }
        closedir(x);
    }
    return initialized;
}

#endif