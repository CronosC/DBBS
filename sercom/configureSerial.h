/*******************************************************
 *            Serial Communication                     *
 *                                                     *
 *                                                     *
 * this code allows communication with a serial device *
 * to be configured using standard POSIX calls         *
 * The code should work at least on Linux and MacOS.   *
 * The only function of interest is:                   *
 *                                                     *
 * int configureSerial(int fd, int baudrate);          *
 * fd       : file handle as returned by open          *
 * baudrate : desired baudrate                         *
 * RETURN   : fd if sucessfull, negative number in     *
 *            case of error (- errno)                  *
 *                                                     *
 ******************************************************/

#ifndef __SERIAL_H__
#define __SERIAL_H__

int configureSerial(int fd, long baudrate); // set baudrate


#endif
