/*******************************************************
 *            Serial Communication                     *
 *                                                     *
 *                                                     *
 * this code allows communication with a serial device *
 * according to POSIX, i.e., it should work at least   *
 * on Linux, Mac, and many other Unix/BSD systems      *
 * around. For Windows, you may want to consult the    *
 * source code of "avrdude" and look into the files    *
 * serial.h and ser_win32.c                            *
 * see: http://www.nongnu.org/avrdude/                 *
 *                                                     *
 * The only function of interest is:                   *
 *                                                     *
 * int configureSerial(int fd, int baudrate);           *
 * fd       : file handle as returned by open          *
 * baudrate : desired baudrate                         *
 * RETURN   : fd if sucessfull, negative number in     *
 *            case of error (- errno)                  *
 *                                                     *
 ******************************************************/

#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

// code for setting baud rate

/* stores baudrate and associated system constant */
struct baud_mapping {
  long baud;
  speed_t speed;
};

/* Lookup table: integer -> baudrate constant */
static struct baud_mapping baud_lookup_table [] = {
  { 1200,   B1200 },
  { 2400,   B2400 },
  { 4800,   B4800 },
  { 9600,   B9600 },
  { 19200,  B19200 },
  { 38400,  B38400 },
  { 57600,  B57600 },
#ifdef B115200
  { 115200, B115200 },
#endif
#ifdef B230400
  { 230400, B230400 },
#endif
  { 0,      0 }                 /* Terminator */
};

/* Utility function for looking up baudrate */
speed_t serialBaudLookup(long baud) {
  struct baud_mapping *map = baud_lookup_table;

  while (map->baud) {
    if (map->baud == baud) /* found */
      return map->speed;
    map++;
  }
  fprintf(stderr,"WARNING: baud rate possibly unsupported, proceeding anyway...\n");
  return baud;
}

// set baud rate on serial port by filedescriptor (as obtained by open())
// on sucess, filedescriptor is returned
// on failure, negative error code is returned and the file descriptor is closed
int configureSerial(int fd, long baudrate) {
  int rc;
  struct termios termios;
  printf("baud rate set to: %li \n", baudrate);
  
  speed_t br = serialBaudLookup(baudrate);
  

  rc = tcgetattr(fd, &termios); // read current terminal settings
  if (rc < 0) {
    fprintf(stderr,"configureSerial(): tcgetattr() failed\n");
    close(fd);
    return -errno;
  }

  termios.c_iflag = IGNBRK;    // configure port (8 bit, ignore break)
  termios.c_oflag = 0;
  termios.c_lflag = 0;
  termios.c_cflag = (CS8 | CREAD | CLOCAL);
  termios.c_cc[VMIN]  = 1;
  termios.c_cc[VTIME] = 0;

  cfsetospeed(&termios, br);   // set speed for outgoing and incoming
  cfsetispeed(&termios, br);

  rc = tcsetattr(fd, TCSANOW, &termios); // write updated structure
  if (rc < 0) {
    fprintf(stderr,"configureSerial(): tcsetattr() failed\n");
    close(fd);
    return -errno;
  }

  /*
   * Everything is now set up for a local line without modem control
   * or flow control, so clear O_NONBLOCK again.
   */
  rc = fcntl(fd, F_GETFL, 0);
  if (rc != -1) {
    fcntl(fd, F_SETFL, rc & ~O_NONBLOCK);
  } else {
      printf("configureSerial(): fcntl() failed\n");
      close(fd);
      return -1;
  }
  return fd;
}

