#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../cJSON/cJSON.h"
// #include "sqlitedb.h"

typedef struct {
  char *id;
  char *topic;
  char *value;
} entity_t;

int entity_get(entity_t *entity, char *str);

#endif
