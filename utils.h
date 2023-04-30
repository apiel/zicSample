#ifndef _UTILS_H_
#define _UTILS_H_

void removeChar(char* str, char garbage)
{

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage)
            dst++;
    }
    *dst = '\0';
}

void replaceChar(char* str, char search, char replace)
{
    for (char* src = str; *src != '\0'; src++) {
        if (*src == search) {
            *src = replace;
        }
    }
}

void removeExtension(char* str)
{
    char* dot = strrchr(str, '.');
    if (dot) {
        *dot = '\0';
    }
}

void copyFile(char* src, char* dst)
{
    FILE* srcFile = fopen(src, "rb");
    FILE* dstFile = fopen(dst, "wb");
    if (srcFile == NULL || dstFile == NULL) {
        return;
    }

    char buf[1024];
    size_t size;
    while ((size = fread(buf, 1, sizeof(buf), srcFile)) > 0) {
        fwrite(buf, 1, size, dstFile);
    }

    fclose(srcFile);
    fclose(dstFile);
}

#endif