#include <stdio.h>
#include "cjson/cJSON.h"

void parse_eq(const char *json_data) {
    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        printf("JSON parsing error.\n");
        return;
    }

    cJSON *error = cJSON_GetObjectItemCaseSensitive(root, "error");
    if (cJSON_IsObject(error)) {
        cJSON *code = cJSON_GetObjectItemCaseSensitive(error, "code");
        cJSON *desc = cJSON_GetObjectItemCaseSensitive(error, "description");
        printf("API Error Detected:\n");
        if (cJSON_IsString(code)) printf("Code        : %s\n", code->valuestring);
        if (cJSON_IsString(desc)) printf("Description : %s\n", desc->valuestring);
        cJSON_Delete(root);
        return;
    }

    cJSON *response = cJSON_GetObjectItemCaseSensitive(root, "response");
    if (!cJSON_IsArray(response)) {
        printf("Missing or invalid 'response' field.\n");
        cJSON_Delete(root);
        return;
    }

    int count = cJSON_GetArraySize(response);
    if (count == 0) {
        printf("Aucune donnée sismique disponible.\n");
        cJSON_Delete(root);
        return;
    }

    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(response, i);
        if (!cJSON_IsObject(item)) continue;
        cJSON *report = cJSON_GetObjectItemCaseSensitive(item, "report");
        if (!cJSON_IsObject(report)) continue;

        cJSON *mag = cJSON_GetObjectItemCaseSensitive(report, "mag");
        cJSON *location = cJSON_GetObjectItemCaseSensitive(report, "location");
        cJSON *type = cJSON_GetObjectItemCaseSensitive(report, "type");

        printf("---- Earthquake %d ----\n", i + 1);
        printf("Magnitude : %.1f\n", mag ? mag->valuedouble : -1.0);
        printf("Location  : %s\n", location ? location->valuestring : "N/A");
        printf("Type      : %s\n\n", type ? type->valuestring : "N/A");
    }

    cJSON_Delete(root);
}

void parse_wf(const char *json_data) {
    cJSON *root = cJSON_Parse(json_data);
    if (!root) {
        printf("JSON parsing error.\n");
        return;
    }

    cJSON *response = cJSON_GetObjectItemCaseSensitive(root, "response");
    if (!cJSON_IsArray(response)) {
        printf("Missing or invalid 'response' field.\n");
        cJSON_Delete(root);
        return;
    }

    cJSON *first_response = cJSON_GetArrayItem(response, 0);
    if (!cJSON_IsObject(first_response)) {
        printf("Invalid first element of 'response'.\n");
        cJSON_Delete(root);
        return;
    }

    cJSON *periods = cJSON_GetObjectItemCaseSensitive(first_response, "periods");
    if (!cJSON_IsArray(periods)) {
        printf("Missing or invalid 'periods' field.\n");
        cJSON_Delete(root);
        return;
    }

    cJSON *first_period = cJSON_GetArrayItem(periods, 0);
    if (!cJSON_IsObject(first_period)) {
        printf("Invalid first element of 'periods'.\n");
        cJSON_Delete(root);
        return;
    }

    cJSON *weather = cJSON_GetObjectItemCaseSensitive(first_period, "weather");
    if (cJSON_IsString(weather) && weather->valuestring != NULL) {
        printf("Weather: %s\n", weather->valuestring);
    } else {
        printf("Missing or invalid 'weather' field.\n");
    }

    cJSON_Delete(root);
}
