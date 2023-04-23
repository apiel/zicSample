#ifndef _FREESOUND_H_
#define _FREESOUND_H_

#include "def.h"

#include <curl/curl.h>

#ifndef FREESOUND_FILE_KEY
#define FREESOUND_FILE_KEY ".freesound.key"
#endif

class Freesound {
protected:
    struct curl_slist* headerlist = NULL;
    bool isEnable = false;

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

public:
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

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://freesound.org/apiv2/search/text/?query=piano&page_size=50&fields=id,name,tags,description,type,filesize,duration,download,previews,num_downloads,avg_rating");
            /* example.com is redirected, so we tell libcurl to follow redirection */
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

            printf("curl_easy_perform() done with code %d (%s).\n", res, curl_easy_strerror(res));

            /* always cleanup */
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