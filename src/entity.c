#include "header/entity.h"

int entity_get(entity_t *entity, char *str) {

  /*
  int ret;
  char header[BUFSIZE];
  char body[BUFSIZE];
  char result[BUFSIZE];
  */

  // time_t t = time(NULL);
  // struct tm *tm = NULL;
  const cJSON *json = NULL;
  const char *id = "ID01";
  const cJSON *topic = NULL; 
  const cJSON *value = NULL; 

  json = cJSON_Parse(str);

  if (json != NULL) {
    topic = cJSON_GetObjectItemCaseSensitive(json, "topic");
    if (!(cJSON_IsString(topic) && (topic->valuestring != NULL))) {
      printf("log failed parse topic.\n");
      return 1;
    }
    // printf("Checking topic \"%s\"\n", topic->valuestring);
    entity->topic = (char*)malloc(strlen(topic->valuestring) + 1);
    memcpy(entity->topic, topic->valuestring, strlen(topic->valuestring) + 1);

    value = cJSON_GetObjectItemCaseSensitive(json, "value");
    if (!(cJSON_IsString(value) && (value->valuestring != NULL))) {
      printf("log failed parse value.\n");
      return 1;
    }
    // printf("Checking value \"%s\"\n", value->valuestring);
    entity->value = (char*)malloc(strlen(value->valuestring) + 1);
    memcpy(entity->value, value->valuestring, strlen(value->valuestring) + 1);
  
    entity->id = (char*)malloc(strlen(id) + 1);
    memcpy(entity->id, id, strlen(id) + 1);
        
    return 0;
  }
  
  const char *error_ptr = cJSON_GetErrorPtr();
  if (error_ptr != NULL) {
    fprintf(stderr, "Error before: %s\n", error_ptr);
    // tm = localtime(&t);
    printf("log failed parse string.\n");
    // printf("%s: log failed parse string.\n", asctime(tm));
    return 1;
  }

  return 1;
    
}
