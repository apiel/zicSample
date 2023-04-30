#ifndef _FREESOUND_DEF_H_
#define _FREESOUND_DEF_H_

#include "def.h"
#include "utils.h"

#include <curl/curl.h>

#ifndef FREESOUND_FILE_KEY
#define FREESOUND_FILE_KEY ".freesound.key"
#endif

#ifndef FREESOUND_OAUTH_FILE_KEY
#define FREESOUND_OAUTH_FILE_KEY ".freesound.oauth.key"
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
    char preview_lq_ogg[128];
    char preview_hq_ogg[128];
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

#endif