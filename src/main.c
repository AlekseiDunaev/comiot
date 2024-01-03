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

// int debug = 0;
       
int main() {

  int res;
  char buf[MAX_BUFFER_LENGHT];
  char message[MAX_BUFFER_LENGHT];
  
  entity_t entity;
  
  int sockfd;
  int last_char_pos = 0;

  int sqlite_ok = 0;
  int influx_ok = 0;
  
  /*
  char buffer_uci[80];
  char debug_path[] = "comiot.config.debug";
  char debug_value[256];
  
  memset(&buffer_uci, '\0', sizeof(buffer_uci));
  get_config_entry(debug_path, buffer_uci);
  sprintf(debug_value, "%s", buffer_uci);
  debug = atoi(debug_value);
  if (debug) { printf("%s: %s\n", debug_path, debug_value); }
  memset(&buffer_uci, '\0', sizeof(buffer_uci));
  */

  // printf("uci read config\n");
  
  init_comport_noblock();
  if ((sockfd = influxdb_connect()) >= 0) {
    influx_ok = 1;
  };
  if (!sqlitedb_test()) {
    sqlite_ok = 1;
  };
  
  memset(message, '\0', sizeof(message));

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

            if (sqlite_ok) {
              sqlitedb_write_message(&entity);
            }

            if (!influx_ok) {
              if ((sockfd = influxdb_connect()) < 0) {
                printf("log Error influx connect\n");
              } else {
                influx_ok = 1;
              }
            }
            
            if (influx_ok) {
              if (influxdb_send_message(&entity, sockfd) < 0) {
                printf("log Error send influx message\n");
              }
            }

          } else {
            printf("log Error read entity\n");
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
