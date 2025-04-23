#include <stdio.h>
#include <string.h>
#include "api.h"
#include "geo.h"

char id[] = "ELlejF0wet64qIfWWimUA";
char secret[] = "kXz0BIX4CCq2qUm87hahetHZGUhk6Eyqk5WUluZx";

char earthquakeAPI[512];
char weatherAPI[512];

curlOut out = { .response = NULL, .size = 0 };

static size_t cb(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    curlOut *mem = (curlOut *)userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (!ptr) return 0;

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

void curlJSON(const char *url, curlOut *out) {
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)out);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
}

void clear_output(curlOut *out) {
    free(out->response);
    out->response = NULL;
    out->size = 0;
}

void buildLink(char *rqType, char *id, char *secret, char *out) {
    sprintf(out, "https://data.api.xweather.com/%s/?client_id=%s&client_secret=%s&p=", rqType, id, secret);
    addLatLon2Link(out);
}
