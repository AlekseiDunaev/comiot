#include "header/sqlitedb.h"

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
  
  rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    printf("log failed to fetch data.\n");
    sqlite3_close(db);
    return 1;
  }    
                              
  rc = sqlite3_step(res);
                                  
  if (rc == SQLITE_ROW) {
    printf("%s\n", sqlite3_column_text(res, 0));
  }

  // sqlite3_finalize(res);
  
  char *sql = "DROP TABLE IF EXISTS Cars;" 
              "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);" 
              "INSERT INTO Cars VALUES(1, 'Audi', 52642);" 
              "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);" 
              "INSERT INTO Cars VALUES(3, 'Skoda', 9000);" 
              "INSERT INTO Cars VALUES(4, 'Volvo', 29000);" 
              "INSERT INTO Cars VALUES(5, 'Bentley', 350000);" 
              "INSERT INTO Cars VALUES(6, 'Citroen', 21000);" 
              "INSERT INTO Cars VALUES(7, 'Hummer', 41400);" 
              "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";
  
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
