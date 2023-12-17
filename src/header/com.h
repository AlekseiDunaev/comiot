#ifndef __COM_H__
#define __COM_H__

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "uciio.h"

/* baudrate settings are defined in <asm/termbits.h>, which is included by <termios.h> */
#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

int pexit(char *msg);
// int read_com(int fd, int len, int timeout, char *buf);
// int init_comport(struct termios *oldtio); 
// int init_comport_noblock(struct termios *oldtio);
// void close_term(int fd, struct termios* oldtio);
int read_com(int len, int timeout, char *buf);
void init_comport_noblock();
void close_term();

#endif 
