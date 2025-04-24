#include "api.h"
#include "geo.h"
#include "parser.h"

int main(void) {

    char *weather_json = NULL;
    char *eq_json = NULL;

    buildLink(WEATHER, NULL, owmKey, weatherAPI);
    curlJSON(weatherAPI, &out);
    parse_wf(out.response,&weather_json);
    clear_output(&out);


    buildLink(EARTHQ, id, secret, earthquakeAPI);
    curlJSON(earthquakeAPI, &out);
    parse_eq(out.response, &eq_json);

//    printf("%s\n", weather_json);
//    printf("%s\n", eq_json);

    merge_jsons(weather_json,eq_json);

    free(out.response);
    return 0;
}

