#ifndef PARSER_H
#define PARSER_H

void parse_eq(const char *json_data, char **output_json);
void parse_wf(const char *json_data, char **output_json);
void merge_jsons(const char *weather_json, const char *eq_json);

#endif
