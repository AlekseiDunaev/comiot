#include "header/sqlitedb.h"

// extern int debug;
sqlite3 *db;
sqlite3_stmt *res;
char *err_msg = 0;

int sqlitedb_test() {

  int rc = sqlite3_open("/root/test.db", &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    printf("log failed open database.\n");
    return 1;
  }
  
  sqlite3_close(db);

  return 0;
                                              
}

int sqlitedb_write_message(entity_t *entity) {
  
  int rc = sqlite3_open("/root/test.db", &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    printf("log failed open database.\n");
    return 1;
  }
  
  char sql[256];

  sprintf(sql, "CREATE TABLE IF NOT EXISTS Sensors(ID INTEGER PRIMARY KEY AUTOINCREMENT,Date_Time TEXT, Topic TEXT, Value TEXT); INSERT INTO Sensors(Date_Time, Topic, Value) VALUES(datetime('now'), '%s', '%s');", entity->topic , entity->value);
  // printf("%s\n", sql);
  
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        
  if (rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);

    return 1;
  } 
            
  sqlite3_close(db);
  
  return 0;  

}
