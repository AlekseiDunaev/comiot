#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
// #include <libconfig.h>

#include <mosquitto.h>
#include <sqlite3.h>
#include "cJSON/cJSON.h"

#include "header/com.h"
#include "header/connection.h"
#include "header/entity.h"
#include "header/sqlitedb.h"

#define MAX_BUFFER_LENGHT 150
       
int main() {

  int res;
  char buf[MAX_BUFFER_LENGHT];
  char message[MAX_BUFFER_LENGHT];
  
  int sockfd;
  int last_char_pos = 0;

  int sqlite_ok = 0;
  
  // printf("%s\n", sqlite3_libversion());

  init_comport_noblock();
  sockfd = influxdb_connect();
  entity_t entity;

  memset(message, '\0', sizeof(message));

  if (!sqlitedb_test()) {
    sqlite_ok = 1;
  };

  while(true) {

    memset(buf, '\0', sizeof(buf));
    
    if (res = read_com(MAX_BUFFER_LENGHT, 2000, buf) != 0) {
      // printf("%s\n", buf);

      for (int i = 0; i < strlen(buf); i++) {
        if (buf[i] == '\n') {
          printf("%s\n", message);

          if (!entity_get(&entity, message)) {
            // printf("id: %s\n", entity.id);
            // printf("topic: %s\n", entity.topic);
            // printf("value: %s\n", entity.value);
            influxdb_send_message(&entity, sockfd);
          } 
					
          last_char_pos = 0;
          memset(message, '\0', sizeof(message));
          break;
        }
        message[last_char_pos] = buf[i];
        last_char_pos++;
      }
    }
  }

  /* Restore the ld port settings */
  close_term();

  return 0;
}
