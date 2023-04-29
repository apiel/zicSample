#ifndef _FREESOUND_OAUTH_H_
#define _FREESOUND_OAUTH_H_

#include "freesoundDef.h"

static size_t freesoundOauthHeaderCallback(char* buffer, size_t size, size_t nitems, void* userdata)
{
    // APP_LOG("freesoundOauthHeaderCallback: %s\n", buffer);
    char * cookie = strstr(buffer, "Set-Cookie:");
    if (cookie) {
        cookie += 12; // skip "Set-Cookie: "
        char* end = strstr(cookie, ";");
        if (end) {
            *end = '\0';
        }
        strcpy((char *)userdata, cookie);
    }
    return nitems * size;
}

class FreesoundOauth {
protected:
    char postFields[512];

    void appendCredential()
    {
        size_t sz;
        void* loaded = SDL_LoadFile(FREESOUND_OAUTH_FILE_KEY, &sz);
        if (!loaded || sz == 0) {
            APP_LOG("Error: could not load freesound credential from %s\n", FREESOUND_OAUTH_FILE_KEY);
            return;
        }
        strcat(postFields, (char*)loaded);
        SDL_free(loaded);
    }

    char* getValue(char* prop)
    {
        if (!prop) {
            return NULL;
        }
        strtok(prop, "\"");
        return strtok(NULL, "\"");
    }

    void parseForm()
    {
        APP_LOG("\n\n\n####\n");

        char* input;
        char* rest = freesoundData;

        strcpy(postFields, "");
        while ((input = strstr(rest, "<input"))) {
            input = strtok_r(input, ">", &rest);
            // First find all props before to get value because strtok set \0
            char* name = strstr(input, "name=\"");
            char* value = strstr(input, "value=\"");
            name = getValue(name);
            value = getValue(value);
            // APP_LOG("# input: %s = %s\n", name, value);
            if (name && value) {
                sprintf(postFields + strlen(postFields), "%s=%s&", name, value);
            }
        }
        appendCredential();
        APP_LOG("postFields: %s\n", postFields);
    }

    void sendForm(char* url, char* cookies)
    {
        if (url == NULL || postFields[0] == '\0') {
            return;
        }
        APP_LOG("----- sendForm %s\n", url);
        CURL* curl = curl_easy_init();
        if (curl) {
            strcpy(freesoundData, "");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            // curl_easy_setopt(curl, CURLOPT_URL, "https://freesound.org/apiv2/login/");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields);
            curl_easy_setopt(curl, CURLOPT_COOKIE, cookies);
            struct curl_slist* headerlist = NULL;
            headerlist = curl_slist_append(headerlist, "Origin: https://freesound.org");
            headerlist = curl_slist_append(headerlist, "Host: freesound.org");

            // char cookiesHeader[512];
            // sprintf(cookiesHeader, "Cookie: %s", cookies);
            // APP_LOG("++++++ set cookies: %s\n", cookiesHeader);
            // headerlist = curl_slist_append(headerlist, cookiesHeader);

            char referer[512];
            sprintf(referer, "Referer: %s", url);
            APP_LOG("++++++ set referer: %s\n", referer);
            headerlist = curl_slist_append(headerlist, referer);

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                // TODO show an error
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                APP_LOG("Oauth Freesound data: %s\n", freesoundData);

                // parseForm();
            }
            curl_easy_cleanup(curl);
        }
    }

public:
    void init()
    {
        CURL* curl = curl_easy_init();
        if (curl) {
            char cookies[512];
            strcpy(cookies, "");
            strcpy(freesoundData, "");
            curl_easy_setopt(curl, CURLOPT_URL, "https://freesound.org/apiv2/oauth2/authorize/?client_id=aOE7UaYW68l205T8RHuH&response_type=code");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            freesoundDataPtr = freesoundData;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, freesoundDataCallback);
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, freesoundOauthHeaderCallback);
            curl_easy_setopt(curl,  CURLOPT_HEADERDATA, cookies);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                // TODO show an error
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                // APP_LOG("Oauth Freesound data: %s\n", freesoundData);

                char* effectiveUrl;
                curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effectiveUrl);

                // APP_LOG("Oauth Freesound cookie: %s\n", cookies);

                parseForm();
                sendForm(effectiveUrl, cookies);
            }
            curl_easy_cleanup(curl);
        }
    }
};

#endif