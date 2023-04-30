#ifndef _FREESOUND_H_
#define _FREESOUND_H_

#include "freesoundDef.h"
#if FREESOUND_OAUTH_ENABLED
#include "freesoundOauth.h"
#endif

class Freesound {
protected:
#if FREESOUND_OAUTH_ENABLED
    FreesoundOauth oauth;
#endif
    struct curl_slist* headerlist = NULL;
    bool isEnable = false;

    int8_t currentItem = -1;

    static Freesound* instance;

    Freesound()
    {
    }

    void loadKey()
    {
        size_t sz;
        void* loaded = SDL_LoadFile(FREESOUND_FILE_KEY, &sz);
        if (!loaded) {
            APP_LOG("Error: could not load freesound key from %s\n", FREESOUND_FILE_KEY);
            return;
        }

        isEnable = strlen((char*)loaded) > 0;
        if (!isEnable) {
            APP_LOG("Error: invalid freesound key\n");
            return;
        }

        char authHeader[70];
        snprintf(authHeader, sizeof(authHeader) * sizeof(authHeader[0]), "Authorization: Token %s", (char*)loaded);
        APP_LOG("Freesound key:::: %s\n", authHeader);
        headerlist = curl_slist_append(headerlist, authHeader);

        SDL_free(loaded);
    }

    char* setChar(char* rest, char* target)
    {
        if (rest[1] == 'n') { // :null
            strcpy(target, "");
        } else {
            strtok_r(rest, "\"", &rest);
            char* field = strtok_r(rest, "\"", &rest);
            strcpy(target, field);
        }
        return rest;
    }

    char* setInt(char* rest, int* target)
    {
        char* field = strtok_r(rest, "\"", &rest) + 1; // +1 to skip :
        *target = atoi(field);
        return rest;
    }

    char* setFloat(char* rest, float* target)
    {
        char* field = strtok_r(rest, "\"", &rest) + 1; // +1 to skip :
        *target = atof(field);
        return rest;
    }

    void parseData()
    {
        char* rest = freesoundData;
        char* field;
        while ((field = strtok_r(rest, "\"", &rest))) {
            // APP_LOG("field: %s\n", field);
            if (strcmp(field, "next") == 0) {
                rest = setChar(rest, nextUrl);
            } else if (strcmp(field, "previous") == 0) {
                rest = setChar(rest, previousUrl);
            } else if (strcmp(field, "count") == 0) {
                rest = setInt(rest, &totalCount);
            } else if (strcmp(field, "id") == 0) {
                currentItem++;
                rest = setInt(rest, &items[currentItem].id);
            } else if (strcmp(field, "name") == 0) {
                rest = setChar(rest, items[currentItem].name);
            } else if (strcmp(field, "tags") == 0) {
                char* tags = strtok_r(rest, "]", &rest) + 2; // +2 to skip :[
                removeChar(tags, '"');
                strcpy(items[currentItem].tags, tags);
                // strcpy(items[currentItem].tags, strtok_r(rest, "]", &rest) + 2); // +2 to skip :[
            } else if (strcmp(field, "filesize") == 0) {
                rest = setInt(rest, &items[currentItem].filesize);
                if (items[currentItem].filesize > 1024 * 1024) {
                    sprintf(items[currentItem].filesizeStr, "%.1fM", items[currentItem].filesize / 1024.0 / 1024.0);
                } else if (items[currentItem].filesize > 1024) {
                    sprintf(items[currentItem].filesizeStr, "%.1fK", items[currentItem].filesize / 1024.0);
                } else {
                    sprintf(items[currentItem].filesizeStr, "%dB", items[currentItem].filesize);
                }
            } else if (strcmp(field, "duration") == 0) {
                rest = setFloat(rest, &items[currentItem].duration);
            } else if (strcmp(field, "download") == 0) {
                rest = setChar(rest, items[currentItem].download);
            } else if (strcmp(field, "preview-lq-ogg") == 0) {
                rest = setChar(rest, items[currentItem].preview_lq_ogg);
            } else if (strcmp(field, "preview-hq-ogg") == 0) {
                rest = setChar(rest, items[currentItem].preview_hq_ogg);
            } else if (strcmp(field, "num_downloads") == 0) {
                rest = setInt(rest, &items[currentItem].num_downloads);
            } else if (strcmp(field, "avg_rating") == 0) {
                rest = setFloat(rest, &items[currentItem].avg_rating);
            }
        }
    }

    CURL* getCurl(char* url)
    {
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        }
        return curl;
    }

public:
    char previousUrl[FREESOUND_SEARCH_URL_SIZE];
    char nextUrl[FREESOUND_SEARCH_URL_SIZE];
    int totalCount = 0;

    FreesoundItem items[FREESOUND_PAGE_SIZE];

    const char* query = "kick";

    static Freesound& get()
    {
        if (!instance) {
            instance = new Freesound();
        }
        return *instance;
    }

    void download(char* url, char* output)
    {
        CURL* curl = getCurl(url);
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, freesoundWriteData);
            FILE* fp = fopen(output, "wb");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                // TODO show an error
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            curl_easy_cleanup(curl);
            fclose(fp);
        }
    }

    void fetch(char* url)
    {
        CURL* curl = getCurl(url);
        if (curl) {
            currentItem = -1;
            freesoundDataPtr = freesoundData;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, freesoundDataCallback);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                // TODO show an error
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                // APP_LOG("Freesound data: %s\n", freesoundData);
                APP_LOG("curl_easy_perform() done with code %d (%s).\n", res, curl_easy_strerror(res));
                parseData();
            }
            curl_easy_cleanup(curl);
        }
    }

    void search()
    {
        char url[FREESOUND_SEARCH_URL_SIZE];
        snprintf(url, FREESOUND_SEARCH_URL_SIZE, FREESOUND_SEARCH_URL, query, FREESOUND_PAGE_SIZE);
        fetch(url);
    }

    int getCount()
    {
        return currentItem + 1;
    }

    void init()
    {
        SDL_Log("Freesound init\n");
        loadKey();

#if FREESOUND_OAUTH_ENABLED
        oauth.init();
#endif

        if (isEnable) {
            search();
        }
    }
};

Freesound* Freesound::instance = nullptr;

#endif