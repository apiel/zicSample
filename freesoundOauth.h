#ifndef _FREESOUND_OAUTH_H_
#define _FREESOUND_OAUTH_H_

#include "freesoundDef.h"

class FreesoundOauth {
protected:
    char* getInputProp(char* input, const char* prop)
    {
        char* propPtr = strstr(input, prop);
        if (!propPtr) {
            return NULL;
        }
        propPtr = strtok(propPtr + strlen(prop), "\"");
        return propPtr;
    }

    void parseForm()
    {
        APP_LOG("\n\n\n####\n");

        char* input;
        char* rest = freesoundData;

        while ((input = strstr(rest, "<input"))) {
            input = strtok_r(input, ">", &rest);
            // APP_LOG("#---- input: %s\n", input);
            
            char* name = strstr(input, "name=\"");
            char* value = strstr(input, "value=\"");

            if (name) {
                name = strtok(name + 6, "\"");
            }
            if (value) {
                value = strtok(value + 7, "\"");
            }


            // char* value = getInputProp(input, "value=\"");
            // char* name = getInputProp(input, "name=\"");
            // APP_LOG("# input: %s = %s\n", name, value);
            APP_LOG("\n");
        }
    }

public:
    void init()
    {
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://freesound.org/apiv2/oauth2/authorize/?client_id=aOE7UaYW68l205T8RHuH&response_type=code");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            freesoundDataPtr = freesoundData;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, freesoundDataCallback);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                // TODO show an error
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                APP_LOG("Oauth Freesound data: %s\n", freesoundData);

                // char* location;
                // res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
                // if ((res == CURLE_OK) && location) {
                //     APP_LOG("Redirected to: %s\n", location);
                // } else {
                //     APP_LOG("No redirect found\n");
                // }
                parseForm();
            }
            curl_easy_cleanup(curl);
        }
    }
};

#endif