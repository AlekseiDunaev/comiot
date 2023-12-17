#include "header/com.h"
  
int fd;
struct termios* oldtio;

int pexit(char *msg) {
  perror(msg);
  exit(1);
}

void init_comport_noblock() {
  struct termios newtio;
	
  //-----------------------------------------------------------------------------
  // Open modem device for reading and writing and not as controlling tty
  // because we don't want to get killed if linenoise sends CTRL-C.
  //-----------------------------------------------------------------------------
  //
  char path[] = "comiot.usbdev.dev";
  char buffer[80];
  memset(&buffer, '\0', sizeof(buffer));

  get_config_entry(path, buffer);
  // printf("uci read config: %s: %s\n", path, buffer);

  fd = open(buffer, O_RDWR | O_NOCTTY | O_NDELAY); 
  if (fd < 0) {
    perror(buffer);
    exit(-1);
  }
  

  /* Configure port reading */
  // fcntl(fd, F_SETFL, 0); 	//read com-port is the bloking
  fcntl(fd, F_SETFL, FNDELAY);  //read com-port not bloking
  // fcntl(fd, F_SETFL, O_NDELAY);  //read com-port not bloking

  tcgetattr(fd, oldtio); 
  
  memset(&newtio, '\0', sizeof(newtio));

  // Enable the receiver and set local mode
  if ((cfsetispeed(&newtio, B9600) < 0) || (cfsetospeed(&newtio, B9600) < 0)) {
        perror("Unable to set baudrate");
        exit(-1);
  }

  newtio.c_cflag |= (CLOCAL | CREAD);
  newtio.c_cflag &= ~PARENB; // Mask the character size to 8 bits, no parity
  newtio.c_cflag &= ~CSTOPB; //one stop bit
  // newtio.c_cflag |= CSTOPB; //two stop bit
  newtio.c_cflag &= ~CSIZE;
  newtio.c_cflag |= CS8; // Select 8 data bits
  newtio.c_cflag &= ~CRTSCTS; // Disable hardware flow control
  newtio.c_oflag &= ~OPOST; // Disable postprocessing

  // Software flow control is disabled
  newtio.c_iflag &= ~(IXON | IXOFF | IXANY);
 
  // Enable data to be processed as raw input
  newtio.c_lflag &= ~(ICANON | ECHO | ISIG);

  // Enable check odd
  newtio.c_iflag &= ~(INPCK);

  // Set the new options for the port
  newtio.c_cc[VMIN]  = 10;
  newtio.c_cc[VTIME] = 5;

  newtio.c_iflag |= IGNBRK;
  newtio.c_iflag &= ~ICRNL;
  newtio.c_oflag &= ~ONLCR;
  newtio.c_lflag &= ~IEXTEN;
  newtio.c_lflag &= ~ECHOE;
  newtio.c_lflag &= ~ECHOK;
#ifndef OPENWRT_ARM
  newtio.c_lflag &= ~ECHOCTL;
  newtio.c_lflag &= ~ECHOKE;
#endif
  
  tcflush(fd, TCIFLUSH);
  tcsetattr(fd, TCSANOW, &newtio);
  
}

int read_com(int len, int timeout, char *buf){
  int ret = 0;
        
  struct pollfd fds;
  fds.fd=fd;
  fds.events = POLLIN;
  poll(&fds, 1, timeout);

  if (fds.revents & POLLIN) {
    ret = read(fd, buf, len);
  }
  
  if (ret < 0) {
    ret = 0;
  }
        
  return ret;
}

void close_term() {
    tcsetattr(fd, TCSANOW, oldtio);
}
