#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#define BUFSIZE 8196

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "com.h"
#include "entity.h"

/* Example of loading stats data into InfluxDB in its Line Protocol format over a network using HTTP POST */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 8086 /* Port number as an integer - web server default is 80 */
#define IP_ADDRESS "217.61.0.4"  /* IP Address as a string */
#define DATABASE "mqttwarn" /* This is the InfluxDB database name */
#define USERNAME "telegraf" /* These are the credentials used to access the database */
#define PASSWORD "Fktrcbq3" /* client endpoint details for a tag: replace with your hostname or use gethostname()*/
#define HOSTNAME "blue"

int influxdb_connect();
int influxdb_send_message(entity_t* entity, int sockfd);

#endif
