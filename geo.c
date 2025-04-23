#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "api.h"
#include "geo.h"
#include "cjson/cJSON.h"

void addLatLon2Link(char link[]) {
    curlOut out = { .response = NULL, .size = 0 };
    curlJSON("http://ip-api.com/json/", &out);

    if (out.response) {
        cJSON *json = cJSON_Parse(out.response);
        if (json) {
            cJSON *lat = cJSON_GetObjectItemCaseSensitive(json, "lat");
            cJSON *lon = cJSON_GetObjectItemCaseSensitive(json, "lon");
            if (cJSON_IsNumber(lat) && cJSON_IsNumber(lon)) {
                char latStr[32], lonStr[32];
                snprintf(latStr, sizeof(latStr), "%f", lat->valuedouble);
                snprintf(lonStr, sizeof(lonStr), "%f", lon->valuedouble);
                strcat(link, latStr);
                strcat(link, ",");
                strcat(link, lonStr);
            }
            cJSON_Delete(json);
        }
        free(out.response);
    }
}
