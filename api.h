#ifndef API_H
#define API_H

#include <stdlib.h>
#include <curl/curl.h>

#define WEATHER "forecasts"
#define EARTHQ "earthquakes"

extern char id[];
extern char secret[];
extern char owmKey[];
extern char earthquakeAPI[512];
extern char weatherAPI[512];

typedef struct curlOut {
    char *response;
    size_t size;
} curlOut;

extern curlOut out;

void curlJSON(const char *url, curlOut *out);
void clear_output(curlOut *out);
void buildLink(char *rqType, char *id, char *secret, char *out);
#endif
