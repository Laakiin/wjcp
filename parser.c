#include <stdio.h>
#include "cjson/cJSON.h"
#include "parser.h"
#include <stdlib.h>
#include "string.h"

void parse_eq(const char *json_data, char **output_json) {
    cJSON *root = cJSON_Parse(json_data);
    const char *error_code = "none";

    cJSON *output = cJSON_CreateObject();
    cJSON *eq_object = cJSON_CreateObject();

    if (!root) {
        error_code = "Invalid JSON format";
    } else {
        cJSON *error = cJSON_GetObjectItem(root, "error");
        if (error && cJSON_IsObject(error)) {
            cJSON *code = cJSON_GetObjectItem(error, "code");
            if (code && cJSON_IsString(code)) {
                error_code = code->valuestring;
            }
        }

        cJSON *response = cJSON_GetObjectItem(root, "response");
        if (cJSON_IsArray(response) && cJSON_GetArraySize(response) > 0) {
            cJSON *firstItem = cJSON_GetArrayItem(response, 0);
            cJSON *report = cJSON_GetObjectItem(firstItem, "report");

            if (report) {
                cJSON *mag = cJSON_GetObjectItem(report, "mag");
                cJSON *type = cJSON_GetObjectItem(report, "type");

                cJSON_AddNumberToObject(eq_object, "mag", cJSON_IsNumber(mag) ? mag->valuedouble : 0);
                cJSON_AddStringToObject(eq_object, "type", cJSON_IsString(type) ? type->valuestring : "none");
            }
        } else {
            cJSON_AddNumberToObject(eq_object, "mag", 0);
            cJSON_AddStringToObject(eq_object, "type", "none");
        }
    }

    cJSON_AddStringToObject(eq_object, "error", error_code);
    cJSON_AddItemToObject(output, "earthquake", eq_object);

    char *formatted_output = cJSON_PrintUnformatted(output);
    *output_json = formatted_output;

    cJSON_Delete(output);
    cJSON_Delete(root);
}

void parse_wf(const char *json_data, char **output_json) {
    cJSON *root = cJSON_Parse(json_data);
    const char *error_msg = "none";

    cJSON *output = cJSON_CreateObject();
    cJSON *weather_object = cJSON_CreateObject();

    if (!root) {
        error_msg = "Invalid JSON format";
    } else {
        cJSON *cod = cJSON_GetObjectItem(root, "cod");

        int cod_val = 0;
        if (cJSON_IsNumber(cod)) {
            cod_val = cod->valueint;
        } else if (cJSON_IsString(cod)) {
            cod_val = atoi(cod->valuestring);
        } else {
            error_msg = "Missing or invalid 'cod' field";
        }

        if (cod_val != 200) {
            cJSON *msg = cJSON_GetObjectItem(root, "message");
            error_msg = (msg && cJSON_IsString(msg)) ? msg->valuestring : "Unknown error";
        } else {
            cJSON *weather_arr = cJSON_GetObjectItem(root, "weather");
            cJSON *main_obj = cJSON_GetObjectItem(root, "main");

            if (weather_arr && cJSON_IsArray(weather_arr) && main_obj) {
                cJSON *first_weather = cJSON_GetArrayItem(weather_arr, 0);
                if (first_weather) {
                    cJSON *main_str = cJSON_GetObjectItem(first_weather, "main");
                    cJSON *desc_str = cJSON_GetObjectItem(first_weather, "description");
                    cJSON *feels_like = cJSON_GetObjectItem(main_obj, "feels_like");

                    cJSON_AddStringToObject(weather_object, "weather",
                        (main_str && cJSON_IsString(main_str)) ? main_str->valuestring : "");
                    cJSON_AddStringToObject(weather_object, "desc",
                        (desc_str && cJSON_IsString(desc_str)) ? desc_str->valuestring : "");
                    cJSON_AddNumberToObject(weather_object, "temp",
                        (feels_like && cJSON_IsNumber(feels_like)) ? feels_like->valuedouble : 0.0);
                }
            } else {
                error_msg = "Missing 'weather' or 'main'";
            }
        }
    }

    cJSON_AddStringToObject(weather_object, "error", error_msg);
    cJSON_AddItemToObject(output, "weather", weather_object);

    char *formatted_output = cJSON_PrintUnformatted(output);
    *output_json = formatted_output;

    cJSON_Delete(output);
    cJSON_Delete(root);
}


void merge_jsons(const char *weather_json, const char *eq_json) {
    cJSON *weather_root = cJSON_Parse(weather_json);
    cJSON *eq_root = cJSON_Parse(eq_json);

    if (!weather_root || !eq_root) {
        printf("Erreur de parsing dans l'un des JSON d'entrée\n");
        if (weather_root) cJSON_Delete(weather_root);
        if (eq_root) cJSON_Delete(eq_root);
        return;
    }

    cJSON *combined = cJSON_CreateObject();
    cJSON *weather_obj = cJSON_GetObjectItem(weather_root, "weather");

    if (weather_obj)
        cJSON_AddItemToObject(combined, "weather", cJSON_Duplicate(weather_obj, 1));

    cJSON *eq_obj = cJSON_GetObjectItem(eq_root, "earthquake");
    if (eq_obj)
        cJSON_AddItemToObject(combined, "earthquake", cJSON_Duplicate(eq_obj, 1));

    char *formatted_output = cJSON_Print(combined);
    printf("%s\n",formatted_output);

    free(formatted_output);
    cJSON_Delete(combined);
    cJSON_Delete(weather_root);
    cJSON_Delete(eq_root);
}


