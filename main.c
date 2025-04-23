#include "api.h"
#include "geo.h"
#include "parser.h"

int main(void) {
    buildLink(WEATHER, id, secret, weatherAPI);
    curlJSON(weatherAPI, &out);
    parse_wf(out.response);
    clear_output(&out);

    buildLink(EARTHQ, id, secret, earthquakeAPI);
    curlJSON(earthquakeAPI, &out);
    parse_eq(out.response);

    free(out.response);
    return 0;
}
l
