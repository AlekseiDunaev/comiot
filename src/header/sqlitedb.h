#ifndef __SQLITEDB_H__
#define __SQLITEDB_H__

#include "entity.h"
#include <sqlite3.h>
#include <stdio.h>

int sqlitedb_test();
int sqlitedb_write_message(entity_t *entity);

#endif

