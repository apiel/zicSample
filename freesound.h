#ifndef _FREESOUND_H_
#define _FREESOUND_H_

#include "def.h"
#include "utils.h"

#include <curl/curl.h>

#ifndef FREESOUND_FILE_KEY
#define FREESOUND_FILE_KEY ".freesound.key"
#endif

#ifndef FREESOUND_PAGE_SIZE
#define FREESOUND_PAGE_SIZE 20
#endif

#ifndef FREESOUND_DATA_SIZE
#define FREESOUND_DATA_SIZE FREESOUND_PAGE_SIZE * 1000
#endif

#ifndef FREESOUND_SEARCH_FIELDS
#define FREESOUND_SEARCH_FIELDS "id,name,tags,filesize,duration,download,previews,num_downloads,avg_rating"
#endif

struct FreesoundItem {
    int id;
    char name[256];
    char tags[256];
    int filesize;
    char filesizeStr[16];
    float duration;
    char download[128];
    char preview_lq_mp3[128];
    int num_downloads;
    float avg_rating;
};

#ifndef FREESOUND_SEARCH_FILTER
#define FREESOUND_SEARCH_FILTER "type:wav"
#endif

#ifndef FREESOUND_SEARCH_URL
#define FREESOUND_SEARCH_URL "https://freesound.org/apiv2/search/text/?query=%s&page_size=%d&fields=" FREESOUND_SEARCH_FIELDS "&filter=" FREESOUND_SEARCH_FILTER
#endif

#ifndef FREESOUND_SEARCH_URL_SIZE
#define FREESOUND_SEARCH_URL_SIZE 512
#endif

// https://niranjanmalviya.wordpress.com/2018/06/23/get-json-data-using-curl/
// https://stackoverflow.com/questions/24884490/using-libcurl-and-jsoncpp-to-parse-from-https-webserver

char freesoundData[FREESOUND_DATA_SIZE];
char* freesoundDataPtr = freesoundData;

static size_t freesoundDataCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;

    if (realsize + freesoundDataPtr - freesoundData > FREESOUND_DATA_SIZE) {
        APP_LOG("Error: freesound data buffer overflow\n");
        return 0;
    }

    memcpy(freesoundDataPtr, contents, realsize);
    freesoundDataPtr += realsize;
    *freesoundDataPtr = 0;

    return realsize;
}

class Freesound {
protected:
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
                rest = setInt(rest, &count);
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
            } else if (strcmp(field, "preview-lq-mp3") == 0) {
                rest = setChar(rest, items[currentItem].preview_lq_mp3);
            } else if (strcmp(field, "num_downloads") == 0) {
                rest = setInt(rest, &items[currentItem].num_downloads);
            } else if (strcmp(field, "avg_rating") == 0) {
                rest = setFloat(rest, &items[currentItem].avg_rating);
            }
        }
    }

public:
    char previousUrl[FREESOUND_SEARCH_URL_SIZE];
    char nextUrl[FREESOUND_SEARCH_URL_SIZE];
    int count = 0;

    FreesoundItem items[FREESOUND_PAGE_SIZE];

    const char* query = "kick";

    static Freesound& get()
    {
        if (!instance) {
            instance = new Freesound();
        }
        return *instance;
    }

    void search()
    {
        CURL* curl;
        CURLcode res;

        currentItem = -1;

        curl = curl_easy_init();
        if (curl) {
            char url[FREESOUND_SEARCH_URL_SIZE];
            snprintf(url, FREESOUND_SEARCH_URL_SIZE, FREESOUND_SEARCH_URL, query, FREESOUND_PAGE_SIZE);
            curl_easy_setopt(curl, CURLOPT_URL, url);
            /* example.com is redirected, so we tell libcurl to follow redirection */
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

            freesoundDataPtr = freesoundData;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, freesoundDataCallback);

            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                // APP_LOG("Freesound data: %s\n", freesoundData);
                APP_LOG("curl_easy_perform() done with code %d (%s).\n", res, curl_easy_strerror(res));
                parseData();
            }
            curl_easy_cleanup(curl);
        }
    }

    void init()
    {
        SDL_Log("Freesound init\n");
        loadKey();

        if (isEnable) {
            search();
        }
    }
};

Freesound* Freesound::instance = nullptr;

#endif