#include "header/connection.h"

char db_ip[256];
char db_port[256];
char db_database[256];
char db_username[256];
char db_password[256];

int influxdb_connect() {
  int sockfd;
  static struct sockaddr_in serv_addr;

  char buffer[80];
  char db_ip_path[] = "comiot.influx_conn.db_ip";
  char db_port_path[] = "comiot.influx_conn.db_port";
    
  // printf("uci read config\n");

  memset(&buffer, '\0', sizeof(buffer));
  get_config_entry(db_ip_path, buffer);
  sprintf(db_ip, "%s", buffer);
  // printf("%s: %s\n", db_ip_path, db_ip);
  memset(&buffer, '\0', sizeof(buffer));
  // bzero(&buffer, sizeof(buffer));

  get_config_entry(db_port_path, buffer);
  sprintf(db_port, "%s", buffer);
  // printf("%s: %s\n", db_port_path, db_port);
  // bzero(&buffer, sizeof(buffer));

  /* static is zero filled on start up */
  // printf("Connecting from uci config file socket to %s and port %d\n", db_ip, atoi(db_port));

  if((sockfd = socket(AF_INET, SOCK_STREAM,0)) <0) pexit("socket() failed");
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(db_ip);
  serv_addr.sin_port = htons(atoi(db_port));
  
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0) pexit("connect() failed");
  // printf("Connected socket to %s and port %d\n", db_ip, atoi(db_port));

  return sockfd;
 }

int influxdb_send_message(entity_t* entity, int sockfd) {
  int ret;
  char header[BUFSIZE];
  char body[BUFSIZE];
  char result[BUFSIZE];
  
  /* InfluxDB line protocol note: measurement name is "noise" tag is host=blue - multiple tags separate with comma data is random=<number> ending epoch time missing (3 spaces) so InfluxDB generates the timestamp */

  /* InfluxDB line protocol note: ending epoch time missing so InfluxDB greates it */
  sprintf(body, "sensors,entity=room,sensor=%s,topic=%s value=%s", entity->id, entity->topic, entity->value);
    
  /* Note spaces are important and the carriage-returns & newlines */
  /* db= is the datbase name, u= the username and p= the password */

  sprintf(header, "POST /write?db=%s&u=%s&p=%s HTTP/1.1\r\nHost: influx:8086\r\nContent-Length: %ld\r\n\r\n", DATABASE, USERNAME, PASSWORD, strlen(body));
  // printf("Send to InfluxDB the POST request bytes=%d \n->|%s|<-\n", strlen(header), header);
  ret = write(sockfd, header, strlen(header));
    
  if (ret < 0) pexit("Write Header request to InfluxDB failed");
    
  // printf("Send to InfluxDB the data bytes=%d\n->|%s|<-\n",strlen(body),body);
  ret = write(sockfd, body, strlen(body));
    
  if (ret < 0) pexit("Write Data Body to InfluxDB failed");
  /* Get back the acknwledgement from InfluxDB */
  /* It worked if you get "HTTP/1.1 204 No Content" and some other fluff */

  ret = read(sockfd, result, sizeof(result));
  if (ret < 0) pexit("Reading the result from InfluxDB failed");

  result[ret] = 0; /* terminate string */
  // printf("Result returned from InfluxDB. Note:204 is Sucess\n->|%s|<-\n", result);

  // printf("\nIn function entity_get after entity activate\n\n");

  return 1;
}

